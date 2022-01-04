# Bugsnag crash monitoring and reporting plugin for Unreal Engine
[![Documentation](https://img.shields.io/badge/documentation-latest-blue.svg)](https://docs.bugsnag.com/platforms/unreal-engine/)

Automatically detect crashes and fatal errors in your Unreal Engine games on Android and iOS, collecting diagnostic information and immediately notifying your development team to help you to understand and resolve issues as fast as possible.

Supports Unreal Engine 4.23 and later.


## Features

* Automatically report crashes and fatal errors
* Report [handled exceptions](https://docs.bugsnag.com/platforms/unreal-engine/#reporting-handled-errors)
* [Log breadcrumbs](https://docs.bugsnag.com/platforms/unreal-engine/#logging-breadcrumbs) which are attached to crash reports and add insight to users' actions
* [Attach user information](https://docs.bugsnag.com/platforms/unreal-engine/#identifying-users) to determine how many people are affected by a crash


## Getting started

1. [Create a Bugsnag account](https://bugsnag.com)
1. Complete the instructions in the [integration guide](https://docs.bugsnag.com/platforms/unreal-engine/) to report crashes from your game
1. Report handled exceptions using [`UBugsnagFunctionLibrary::Notify()`](https://docs.bugsnag.com/platforms/unreal-engine/#reporting-handled-errors)
1. Customize your integration using the [configuration options](https://docs.bugsnag.com/platforms/unreal-engine/configuration-options/)


## Support

* [Read the integration guide](https://docs.bugsnag.com/platforms/unreal-engine/) or [configuration options documentation](https://docs.bugsnag.com/platforms/unreal-engine/configuration-options/)
* [Search open and closed issues](https://github.com/bugsnag/bugsnag-unreal/issues?utf8=✓&q=is%3Aissue) for similar problems
* [Report a bug or request a feature](https://github.com/bugsnag/bugsnag-unreal/issues/new)


## Repository organization

The Bugsnag plugin is located in [`Plugins/Bugsnag`](Plugins/Bugsnag).

The top level of this repository contains an example project which uses the Bugsnag plugin - [`BugsnagExample.uproject`](BugsnagExample.uproject).

To build the example project for Android or iOS, you must first run `make` in the top level directory to prepare the dependencies.
This step requires macOS with the Android NDK & SDK and Xcode.


## License

The Bugsnag Unreal Engine notifier is free software released under the MIT License.
See the [LICENSE](LICENSE) for details.
