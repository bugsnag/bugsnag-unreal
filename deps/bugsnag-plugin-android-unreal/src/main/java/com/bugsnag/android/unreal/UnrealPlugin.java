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
    if (client == null || name == null) {
      return;
    }
    if (discardClasses != null) {
      for (Pattern pattern : discardClasses) {
        if (pattern.matcher(name).matches()) {
          return;
        }
      }
    }

    Throwable exc = new RuntimeException();
    exc.setStackTrace(stacktrace);

    client.notify(exc, new OnErrorCallback() {
      @Override
      public boolean onError(Event event) {
        setSeverityReason(event, DEFAULT_HANDLED_REASON);
        List<Error> errors = event.getErrors();

        if (!errors.isEmpty()) {
          Error error = event.getErrors().get(0);
          error.setErrorClass(name);
          error.setErrorMessage(message);

          for (Error err : errors) {
            err.setType(ErrorType.C);
          }
        }
        return runNotifyCallback(event, userdata);
      }
    });
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
