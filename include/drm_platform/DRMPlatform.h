/* Copyright 2026 RDK Management. SPDX-License-Identifier: Apache-2.0 */
#ifndef DRM_PLATFORM_DRM_PLATFORM_H
#define DRM_PLATFORM_DRM_PLATFORM_H

#include "SOCFuncs.h"

#include <mutex>

#if defined(__GNUC__)
#define DRM_PLATFORM_API __attribute__((visibility("default")))
#else
#define DRM_PLATFORM_API
#endif

namespace drm_platform {

class DRM_PLATFORM_API DRMPlatform final {
public:
    static DRMPlatform& instance();

    /*
     * Call at DRM integration startup, before creating sessions or workers.
     * The first call fixes the logger and registers the SoC callbacks once.
     * True means registration succeeded. False leaves all defaults available;
     * the caller decides whether its product can continue with those defaults.
     * Later calls return the original result and do not replace the logger.
     */
    bool init(LogCallback log = nullptr, void* logContext = nullptr);

    void getDrmOEMContext(void** context);
    void getDrmPlatformInitData(void** initData);
    bool isAudioNeedNonSVPContext();
    bool isVideoResCheckNeed();
    bool isDynamicSvpEncryptionEnabled();
    bool loadRevocationList();
    void getDrmStoragePath(std::string& readDir, std::string& storagePath,
                           std::string& storeLocation);
    void platformInitializePlayready();
    void platformInitializeWidevine();
    void platformUninitializePlayready();
    void platformUninitializeWidevine();
    bool isSecureClockInitNeed();

    DRMPlatform(const DRMPlatform&) = delete;
    DRMPlatform& operator=(const DRMPlatform&) = delete;

private:
    DRMPlatform() = default;
    static void forwardLog(void* context, LogLevel level, const char* message);

    std::once_flag initFlag_;
    SOCFuncs socFuncs_{};
    LogCallback logSink_{nullptr};
    void* logSinkContext_{nullptr};
    bool socLoaded_{false};
    /* Retained until process exit so registered callbacks stay valid. */
    void* socLibrary_{nullptr};
};

} // namespace drm_platform
#endif
