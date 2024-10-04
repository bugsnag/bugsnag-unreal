package com.bugsnag.android.unreal;

import com.bugsnag.android.Breadcrumb;
import com.bugsnag.android.Client;
import com.bugsnag.android.Configuration;
import com.bugsnag.android.Error;
import com.bugsnag.android.ErrorType;
import com.bugsnag.android.Event;
import com.bugsnag.android.OnBreadcrumbCallback;
import com.bugsnag.android.OnErrorCallback;
import com.bugsnag.android.OnSendCallback;
import com.bugsnag.android.OnSessionCallback;
import com.bugsnag.android.Plugin;
import com.bugsnag.android.Session;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Pattern;
import android.util.Log;

public class UnrealPlugin implements Plugin {
  static final String DEFAULT_HANDLED_REASON = "handledError";
  static boolean loaded = false;
  static Client client = null;

  OnBreadcrumbCallback onBreadcrumbRunner = new OnBreadcrumbCallback() {
    @Override
    public boolean onBreadcrumb(Breadcrumb crumb) {
      return runBreadcrumbCallbacks(crumb);
    }
  };

  OnSendCallback onSendRunner = new OnSendCallback() {
    @Override
    public boolean onSend(Event event) {
      // if for some reason the plugin is unloaded when a callback is invoked, skip processing
      return loaded ? runEventCallbacks(event) : true;
    }
  };

  OnSessionCallback onSessionRunner = new OnSessionCallback() {
    @Override
    public boolean onSession(Session session) {
      return runSessionCallbacks(session);
    }
  };

  // Native functions are implemented in the Android layer of the C++ plugin

  /**
   * Run all registered onBreadcrumb callbacks
   *
   * @param crumb The crumb
   * @return true if the breadcrumb should be kept
   */
  static native boolean runBreadcrumbCallbacks(Breadcrumb crumb);

  /**
   * Run all registered onSession callbacks
   *
   * @param session The session
   * @return true if the session should be kept
   */
  static native boolean runSessionCallbacks(Session session);

  /**
   * Run all registered onSend callbacks
   *
   * @param event The event
   * @return true if the event should be kept
   */
  static native boolean runEventCallbacks(Event event);

  /**
   * Run native notify callback
   *
   * @param event The event
   * @param userdata The userdata object passed to notify()
   */
  static native boolean runNotifyCallback(Event event, ByteBuffer userdata);

  /**
   * Set the severity reason based on a String
   * Workaround for lacking a public setter
   *
   * @param event The event
   * @param reasonType one of the constants in SeverityReason class
   */
  static native void setSeverityReason(Event event, String reasonType);

  private static Set<Pattern> discardClasses;


  public UnrealPlugin(Configuration config) {
    config.addOnBreadcrumb(onBreadcrumbRunner);
    config.addOnSession(onSessionRunner);
    config.addOnSend(onSendRunner);

    discardClasses = config.getDiscardClasses();
  }

  public void load(Client client) {
    this.client = client;
    loaded = true;
  }

  public void unload() {
    if (this.client != null) {
      this.client.removeOnBreadcrumb(onBreadcrumbRunner);
      this.client.removeOnSession(onSessionRunner);
      this.client = null;
    }
    loaded = false;
  }


  static void notify(String name, String message, StackTraceElement[] stacktrace, ByteBuffer userdata) {
    try {
      Log.e("RICHLOG NotifyMethod", "notify called with name: " + name + ", message: " + message);

      if (client == null || name == null) {
        Log.e("RICHLOG NotifyMethod", "client is null or name is null. Returning early.");
        return;
      }

      if (discardClasses != null) {
        for (Pattern pattern : discardClasses) {
          if (pattern.matcher(name).matches()) {
            Log.e("RICHLOG NotifyMethod", "Name matches discard pattern: " + pattern.toString() + ". Returning.");
            return;
          }
        }
      }

      Throwable exc = new RuntimeException();
      exc.setStackTrace(stacktrace);
      Log.e("RICHLOG NotifyMethod", "Exception created and stack trace set.");

      try {
        client.notify(exc, new OnErrorCallback() {
          @Override
          public boolean onError(Event event) {
            try {
              Log.e("RICHLOG NotifyMethod", "onError called with event.");
              setSeverityReason(event, DEFAULT_HANDLED_REASON);
              Log.e("RICHLOG NotifyMethod", "Severity reason set to " + DEFAULT_HANDLED_REASON);

              List<Error> errors = event.getErrors();
              if (!errors.isEmpty()) {
                Error error = errors.get(0);
                error.setErrorClass(name);
                error.setErrorMessage(message);
                Log.e("RICHLOG NotifyMethod", "Error class set to " + name + " and message set to " + message);

                for (Error err : errors) {
                  err.setType(ErrorType.C);
                  Log.e("RICHLOG NotifyMethod", "Error type set to C for error.");
                }
              }
              return runNotifyCallback(event, userdata);
            } catch (Exception e) {
              Log.e("RICHLOG NotifyMethod", "Exception in onError: " + e.getMessage(), e);
              return false;
            }
          }
        });
      } catch (Exception e) {
        Log.e("RICHLOG NotifyMethod", "Exception in client.notify: " + e.getMessage(), e);
      }

      Log.e("RICHLOG NotifyMethod", "Notification sent to Bugsnag.");
    } catch (Exception e) {
      Log.e("RICHLOG NotifyMethod", "Exception in notify method: " + e.getMessage(), e);
    }
  }



  static byte[] getMetadata(Event event, String section, String key) throws IOException {
    return section == null || key == null ? null : wrapAndSerialize(event.getMetadata(section, key));
  }

  static byte[] getMetadata(String section, String key) throws IOException {
    return section == null || key == null || client == null ? null : wrapAndSerialize(client.getMetadata(section, key));
  }

  static byte[] getMetadata(Event event, String section) throws IOException {
    return section == null ? null : serialize(event.getMetadata(section));
  }

  static byte[] getMetadata(String section) throws IOException {
    return section == null || client == null ? null : serialize(client.getMetadata(section));
  }

  private static byte[] serialize(Map<String, Object> metadata) throws IOException {
    return metadata == null ? null : MetadataSerializer.serialize(metadata);
  }

  private static byte[] wrapAndSerialize(Object value) throws IOException {
    // wrap the value to differentiate between an error and a literal null
    return MetadataSerializer.serialize(new HashMap<String, Object>() {{
        put("Value", value);
    }});
  }
}
