package com.bugsnag.android.unreal;

import com.bugsnag.android.Client;
import com.bugsnag.android.Configuration;
import com.bugsnag.android.Breadcrumb;
import com.bugsnag.android.OnBreadcrumbCallback;
import com.bugsnag.android.Plugin;

public class UnrealPlugin implements Plugin {
  Client client = null;
  OnBreadcrumbCallback onBreadcrumbRunner = new OnBreadcrumbCallback() {
    @Override
    public boolean onBreadcrumb(Breadcrumb crumb) {
      return runBreadcrumbCallbacks(crumb);
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

  public void load(Client client) {
    this.client = client;
    this.client.addOnBreadcrumb(onBreadcrumbRunner);
  }

  public void unload() {
    if (this.client != null) {
      this.client.removeOnBreadcrumb(onBreadcrumbRunner);
      this.client = null;
    }
  }
}
