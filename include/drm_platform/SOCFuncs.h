/* Copyright 2026 RDK Management. SPDX-License-Identifier: Apache-2.0 */
#ifndef DRM_PLATFORM_SOC_FUNCS_H
#define DRM_PLATFORM_SOC_FUNCS_H

#include <cstdint>
#include <string>

#if defined(__GNUC__)
#define DRM_PLATFORM_SOC_EXPORT __attribute__((visibility("default")))
#else
#define DRM_PLATFORM_SOC_EXPORT
#endif

namespace drm_platform {

constexpr std::uint32_t SOC_FUNCS_ABI_VERSION = 1;

enum class LogLevel : std::uint32_t { Error, Warning, Info, Debug };
using LogCallback = void (*)(void* context, LogLevel level, const char* message);

/*
 * The common library sets the header and logging fields before registration.
 * The SoC sets only its callbacks; a null callback selects the common default.
 * This is a C++ contract: both libraries must use compatible toolchains and
 * standard-library ABIs, including the same std::string ABI.
 */
struct SOCFuncs {
    std::uint32_t abiVersion;
    std::uint32_t structSize;
    LogCallback log;
    void* logContext;

    void (*getDrmOEMContext)(void** context);
    void (*getDrmPlatformInitData)(void** initData);
    bool (*isAudioNeedNonSVPContext)();
    bool (*isVideoResCheckNeed)();
    bool (*isDynamicSvpEncryptionEnabled)();
    bool (*loadRevocationList)();
    void (*getDrmStoragePath)(std::string& readDir,
                              std::string& storagePath,
                              std::string& storeLocation);
    void (*platformInitializePlayready)();
    void (*platformInitializeWidevine)();
    void (*platformUninitializePlayready)();
    void (*platformUninitializeWidevine)();
    bool (*isSecureClockInitNeed)();
};

using InitDRMPlatformSOC = bool (*)(SOCFuncs* socFuncs);

} // namespace drm_platform

/* The single symbol resolved by dlsym. Registration must not initialize DRM. */
extern "C" DRM_PLATFORM_SOC_EXPORT bool initDRMPlatformSOC(
    drm_platform::SOCFuncs* socFuncs);

#endif
