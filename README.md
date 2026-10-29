# DRM Extensions (`drm-ext`)

> **Status: Under Development — Draft Version**

## Overview

The `drm-ext` repository is currently under development. It is intended to provide a common interface layer for DRM-specific functionality within the RDK ecosystem.

The goal is to separate DRM-specific interfaces and platform hooks from the common Secure Video Path (SVP) functionality, enabling a cleaner and more maintainable architecture across DRM integrations and SoC platforms.

## Development Status

This repository is currently in its **draft stage**.

The initial framework implementation, is being prepared for integration into this repository. The code has not yet been fully merged, reviewed, and validated as part of the `drm-ext` project.

Until the initial integration and validation are complete:

- The repository structure and APIs may change.
- The implementation is not considered feature-complete.
- The repository is not intended for production use.
- Integration and usage instructions may be incomplete or subject to change.

## Planned Scope

The intended scope of this repository includes:

- Common interfaces for DRM-specific platform functionality.
- Separation of DRM-specific interfaces from common SVP functionality.
- Support for integration with DRM implementations such as PlayReady and Widevine.
- Interfaces for platform-dependent DRM initialization and related operations.
- A maintainable abstraction layer that supports multiple SoC platforms.

The final scope and API design will be confirmed as development and integration progress.

## Relationship with SVP

The `drm-ext` repository is part of the broader effort to separate DRM-specific functionality from common SVP functionality.

- **`drm-ext`:** Intended to contain common DRM-specific interfaces and platform hooks.
- **`gst-svp-ext-common`:** Intended to provide common Secure Video Path interfaces and functionality.

The interfaces and dependencies between these components will be finalized during the ongoing alignment and integration work.

## Contributions and Feedback

Feedback and technical discussions are welcome as the architecture and APIs evolve.

Please note that interfaces, implementation details, and repository organization may change during this development phase.

## Disclaimer

This repository is a work in progress. The current contents, where available, represent a draft implementation and should not be considered a stable or production-ready release.

A stable version will be communicated once the initial implementation has been integrated, reviewed, and validated.
