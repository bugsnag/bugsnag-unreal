Changelog
=========

## TBD 

* Removed the bugsnag android gradle plugin due to incompatibility with UE 5.1+. Symbols can now be uploaded via the [BugSnag CLI](https://docs.bugsnag.com/platforms/unreal-engine/showing-full-stacktraces/#android-proguard-and-ndk-mappings)[#220](https://github.com/bugsnag/bugsnag-unreal/pull/220)

## TBD

* Updates the bugsnag-cocoa dependency from v6.26.2 to [v6.28.0](https://github.com/bugsnag/bugsnag-cocoa/blob/master/CHANGELOG.md#6280-2023-12-13)

## 1.8.0 (2023-08-28)

* Added support for Unreal Engine 5.2
* Updates the bugsnag-cocoa dependency from v6.25.2 to [v6.26.2](https://github.com/bugsnag/bugsnag-cocoa/blob/master/CHANGELOG.md#6262-2023-04-20)
* Updates the bugsnag-android dependency from v5.28.4 to [v5.30.0](https://github.com/bugsnag/bugsnag-android/blob/master/CHANGELOG.md#5300-2023-05-11)
* Increased MaxBreadcrumb limit from 100 to 500 [#212](https://github.com/bugsnag/bugsnag-unreal/pull/212)

## 1.7.0 (2022-03-13)

* Add support for Unreal Engine v5.1.1
* Updates the bugsnag-android dependency from v5.28.1 to [v5.28.4](https://github.com/bugsnag/bugsnag-android/blob/master/CHANGELOG.md#5284-2023-02-08)
* Updates the bugsnag-cocoa dependency from v6.25.0 to [v6.25.2](https://github.com/bugsnag/bugsnag-cocoa/blob/master/CHANGELOG.md#6252-2023-01-18)

## 1.6.0 (2022-11-09)

* Adds the `Telemetry.Usage` configuration property. [#194](https://github.com/bugsnag/bugsnag-unreal/pull/194)
* Adds the `MaxStringValueLength` configuration property. [#193](https://github.com/bugsnag/bugsnag-unreal/pull/193)
* Updates the package branding

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
