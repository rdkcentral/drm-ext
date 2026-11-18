/* Copyright 2026 RDK Management. SPDX-License-Identifier: Apache-2.0 */
#include "drm_platform/SOCFuncs.h"

namespace {
drm_platform::LogCallback commonLog = nullptr;
void* commonLogContext = nullptr;

bool dynamicSvpEnabled()
{
    commonLog(commonLogContext, drm_platform::LogLevel::Debug,
              "example dynamic SVP query");
    return true;
}

bool videoResolutionCheck()
{
    commonLog(commonLogContext, drm_platform::LogLevel::Debug,
              "example resolution query");
    return false; // A false result is final; it must not trigger fallback.
}
} // namespace

extern "C" DRM_PLATFORM_SOC_EXPORT bool initDRMPlatformSOC(
    drm_platform::SOCFuncs* socFuncs)
{
    if (socFuncs == nullptr ||
        socFuncs->abiVersion != drm_platform::SOC_FUNCS_ABI_VERSION ||
        socFuncs->structSize != sizeof(drm_platform::SOCFuncs) ||
        socFuncs->log == nullptr) {
        return false;
    }

    commonLog = socFuncs->log;
    commonLogContext = socFuncs->logContext;
    socFuncs->isDynamicSvpEncryptionEnabled = dynamicSvpEnabled;
    socFuncs->isVideoResCheckNeed = videoResolutionCheck;
    commonLog(commonLogContext, drm_platform::LogLevel::Info,
              "example SoC registration");

    // Test fixture: partially filled tables must be discarded on failure.
#ifdef DRM_PLATFORM_TEST_REJECT_REGISTRATION
    return false;
#else
    return true;
#endif
}
