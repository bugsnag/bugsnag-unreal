# Contributing

Thanks for stopping by! This document should cover most topics surrounding contributing to `bugsnag-unreal`.

* [How to contribute](#how-to-contribute)
  * [Reporting issues](#reporting-issues)
  * [Fixing issues](#fixing-issues)
  * [Adding features](#adding-features)
* [Architecture](#architecture)
* [Releasing](#releasing)

## Reporting issues

Are you having trouble getting started? Please [contact us directly](mailto:support@bugsnag.com?subject=%5BGitHub%5D%20Cocos2d%20-%20having%20trouble%20getting%20started%20with%20Bugsnag) for assistance with integrating Bugsnag into your application.
If you have spotted a problem with this module, feel free to open a [new issue](https://github.com/bugsnag/bugsnag-unreal/issues/new?template=Bug_report.md). Here are a few things to check before doing so:

* Are you using the latest version of `bugsnag-unreal`? If not, does updating to the latest version fix your issue?
* Has somebody else [already reported](https://github.com/bugsnag/bugsnag-unreal/issues?utf8=%E2%9C%93&q=is%3Aissue%20is%3Aopen) your issue? Feel free to add additional context to or check-in on an existing issue that matches your own.
* Is your issue caused by this module? Only things related to the `bugsnag-unreal` module should be reported here. For anything else, please [contact us directly](mailto:support@bugsnag.com) and we'd be happy to help you out.

### Fixing issues

If you've identified a fix to a new or existing issue, we welcome contributions!
Here are some helpful suggestions on contributing that help us merge your PR quickly and smoothly:

* [Fork](https://help.github.com/articles/fork-a-repo) the
  [library on GitHub](https://github.com/bugsnag/bugsnag-unreal)
* Build and test your changes.
* Commit and push until you are happy with your contribution
* [Make a pull request](https://help.github.com/articles/using-pull-requests)
* Ensure the automated checks pass (and if it fails, please try to address the cause)

### Adding features

Unfortunately we’re unable to accept PRs that add features or refactor the library at this time.
However, we’re very eager and welcome to hearing feedback about the library so please contact us directly to discuss your idea, or open a
[feature request](https://github.com/bugsnag/bugsnag-unreal/issues/new?template=Feature_request.md) to help us improve the library.

Here’s a bit about our process designing and building the Bugsnag libraries:

* We have an internal roadmap to plan out the features we build, and sometimes we will already be planning your suggested feature!
* Our open source libraries span many languages and frameworks so we strive to ensure they are idiomatic on the given platform, but also consistent in terminology between platforms. That way the core concepts are familiar whether you adopt Bugsnag for one platform or many.
* Finally, one of our goals is to ensure our libraries work reliably, even in crashy, multi-threaded environments. Oftentimes, this requires an intensive engineering design and code review process that adheres to our style and linting guidelines.

## Architecture

The `bugsnag-unreal` [Plugin](https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/Plugins/) provides:

* Native crash reporting for Android and iOS
* An Unreal Engine C++ interface for initialization, logging breadcrumb, reporting non-fatal errors, recording report diagnostics, and session management

The root directory of this repository contains an example project that demonstrates use of the Bugsnag Plugin, which itself is located in [`Plugins/Bugsnag`](Plugins/Bugsnag).

To build the example project for Android or iOS, you must first run `make` in the top level directory to prepare the Plugin's dependencies.
This step requires macOS with the Android NDK & SDK and Xcode.

## Releasing

### Making the release

1. Update the changelog with any missing new features and fixes
2. Bump the version number and create a pull request by running `make prerelease VERSION=[number]`
3. Merge the pull request once approved and manual testing of the release has passed
4. Run `make release VERSION=[number]` to tag and draft a new release on GitHub
5. Wait for the `main` branch CI build, and download the `Bugsnag-*-src.zip` artifacts
6. Edit the release on GitHub, copying the changelog entry and uploading the Plugin artifacts

#### Post-release checks

- [ ] Have all Docs PRs been merged?
- [ ] Do the installation instructions work using the released artifact?
- [ ] Can a freshly created example app send an error report from a Shipping build, using the released artifact?
