Changelog
=========

## TBD

* Adds the `MaxStringValueLength` configuration property. [#193](https://github.com/bugsnag/bugsnag-unreal/pull/193)

## 1.5.1 (2022-10-27)

* Updates the bugsnag-cocoa dependency from v6.19.0 to [v6.25.0](https://github.com/bugsnag/bugsnag-cocoa/blob/master/CHANGELOG.md#6240-2022-10-05)
* Updates the bugsnag-android dependency from v5.24.0 to [v5.28.1](https://github.com/bugsnag/bugsnag-android/blob/master/CHANGELOG.md#5281-2022-10-19)

## 1.5.0 (2022-07-06)

* Adds the `Telemetry` configuration property.
* Updates the bugsnag-android dependency from v5.22.1 to [v5.24.0](https://github.com/bugsnag/bugsnag-android/blob/master/CHANGELOG.md#5240-2022-06-30)
* Updates the bugsnag-cocoa dependency from v6.16.8 to [v6.19.0](https://github.com/bugsnag/bugsnag-cocoa/blob/master/CHANGELOG.md#6190-2022-06-29)

## 1.4.0 (2022-05-11)

* Adds official support for Unreal Engine 5.0 🚀
* Fixes improper use of `TCHAR_TO_UTF8()` that could result in use-after-free on Android.
  [#157](https://github.com/bugsnag/bugsnag-unreal/pull/157)
* Updates the bugsnag-android dependency from v5.22.0 to [v5.22.1](https://github.com/bugsnag/bugsnag-android/blob/master/CHANGELOG.md#5221-2022-04-28)
* Updates the bugsnag-cocoa dependency from v6.16.6 to [v6.16.8](https://github.com/bugsnag/bugsnag-cocoa/blob/master/CHANGELOG.md#6168-2022-05-04)

## 1.3.1 (2022-04-26)

* Fixes `java.lang.ClassNotFoundException` in minified Android builds.
  [#151](https://github.com/bugsnag/bugsnag-unreal/pull/151)

## 1.3.0 (2022-04-11)

* Adds the `MaxReportedThreads` configuration property (Android only.)
  [#144](https://github.com/bugsnag/bugsnag-unreal/pull/144)
* Updates the bugsnag-android dependency from v5.19.2 to [v5.22.0](https://github.com/bugsnag/bugsnag-android/blob/master/CHANGELOG.md#5220-2022-03-31)
* Updates the bugsnag-cocoa dependency from v6.16.1 to [v6.16.6](https://github.com/bugsnag/bugsnag-cocoa/blob/master/CHANGELOG.md#6166-2022-04-06)

## 1.2.0 (2022-02-17)

* Adds support for macOS as a target platform.
  [#136](https://github.com/bugsnag/bugsnag-unreal/pull/136)

## 1.1.0 (2022-02-02)

* New APIs to support forthcoming feature flag and experiment functionality.
  For more information, please see https://docs.bugsnag.com/product/features-experiments
  [#123](https://github.com/bugsnag/bugsnag-unreal/pull/123)

* Updates the bugsnag-android dependency from v5.16.0 to [v5.19.2](https://github.com/bugsnag/bugsnag-android/blob/master/CHANGELOG.md#5192-2022-01-31)
* Updates the bugsnag-cocoa dependency from v6.14.3 to [v6.16.1](https://github.com/bugsnag/bugsnag-cocoa/blob/master/CHANGELOG.md#6161-2022-01-19)

## 1.0.1 (2022-01-04)

* Plugin is no longer flagged as "beta"

## 1.0.0 (2021-12-17)

This is the initial public release of Bugsnag for Unreal Engine, supporting Android and iOS as target platforms.

Documentation: https://docs.bugsnag.com/platforms/unreal-engine/

Download: https://www.unrealengine.com/marketplace/en-US/product/bugsnag

🚀
