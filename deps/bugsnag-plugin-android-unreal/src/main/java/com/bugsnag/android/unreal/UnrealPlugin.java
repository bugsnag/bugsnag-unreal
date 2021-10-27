package com.bugsnag.android.unreal;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import com.bugsnag.android.Bugsnag;
import com.bugsnag.android.Client;
import com.bugsnag.android.Configuration;
import com.bugsnag.android.Breadcrumb;
import com.bugsnag.android.Event;
import com.bugsnag.android.OnBreadcrumbCallback;
import com.bugsnag.android.OnSessionCallback;
import com.bugsnag.android.Plugin;
import com.bugsnag.android.Session;

public class UnrealPlugin implements Plugin {
  Client client = null;
  OnBreadcrumbCallback onBreadcrumbRunner = new OnBreadcrumbCallback() {
    @Override
    public boolean onBreadcrumb(Breadcrumb crumb) {
      return runBreadcrumbCallbacks(crumb);
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
   * @return true if the sessioncrumb should be kept
   */
  static native boolean runSessionCallbacks(Session session);

  public void load(Client client) {
    this.client = client;
    this.client.addOnBreadcrumb(onBreadcrumbRunner);
    this.client.addOnSession(onSessionRunner);
  }

  public void unload() {
    if (this.client != null) {
      this.client.removeOnBreadcrumb(onBreadcrumbRunner);
      this.client.removeOnSession(onSessionRunner);
      this.client = null;
    }
  }

  static byte[] getMetadata(Event event, String section, String key) throws IOException {
    return section == null || key == null ? null : wrapAndSerialize(event.getMetadata(section, key));
  }

  static byte[] getMetadata(String section, String key) throws IOException {
    return section == null || key == null ? null : wrapAndSerialize(Bugsnag.getMetadata(section, key));
  }

  static byte[] getMetadata(Event event, String section) throws IOException {
    return section == null ? null : serialize(event.getMetadata(section));
  }

  static byte[] getMetadata(String section) throws IOException {
    return section == null ? null : serialize(Bugsnag.getMetadata(section));
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
