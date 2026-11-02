/* Copyright 2026 RDK Management. SPDX-License-Identifier: Apache-2.0 */
#include "drm_platform/DRMPlatform.h"
#include "DefaultDRMPlatform.h"

#include <cstdio>
#include <dlfcn.h>

#ifndef DRM_PLATFORM_SOC_LIBRARY_PATH
#define DRM_PLATFORM_SOC_LIBRARY_PATH "/usr/lib/libDRMPlatformSoC.so"
#endif

namespace drm_platform {
namespace {
const DefaultDRMPlatform defaultImpl;

void defaultLog(void*, LogLevel level, const char* message)
{
    std::fprintf(stderr, "[DRMPlatform:%u] %s\n",
                 static_cast<unsigned>(level), message);
}
} // namespace

DRMPlatform& DRMPlatform::instance()
{
    static DRMPlatform platform;
    return platform;
}

void DRMPlatform::forwardLog(void* context, LogLevel level, const char* message)
{
    const auto* platform = static_cast<const DRMPlatform*>(context);
    platform->logSink_(platform->logSinkContext_, level, message);
}

bool DRMPlatform::init(LogCallback log, void* logContext)
{
    std::call_once(initFlag_, [this, log, logContext] {
        logSink_ = log != nullptr ? log : defaultLog;
        logSinkContext_ = logContext;

        void* library = dlopen(DRM_PLATFORM_SOC_LIBRARY_PATH, RTLD_NOW | RTLD_LOCAL);
        if (library == nullptr) {
            forwardLog(this, LogLevel::Warning, dlerror());
            return;
        }

        dlerror();
        const auto registerSoc = reinterpret_cast<InitDRMPlatformSOC>(
            dlsym(library, "initDRMPlatformSOC"));
        const char* error = dlerror();
        if (error != nullptr || registerSoc == nullptr) {
            forwardLog(this, LogLevel::Error,
                       error != nullptr ? error : "initDRMPlatformSOC is null");
            dlclose(library);
            return;
        }

        // Publish nothing until registration has succeeded completely.
        SOCFuncs candidate{};
        candidate.abiVersion = SOC_FUNCS_ABI_VERSION;
        candidate.structSize = sizeof(SOCFuncs);
        candidate.log = forwardLog;
        candidate.logContext = this;
        if (!registerSoc(&candidate) ||
            candidate.abiVersion != SOC_FUNCS_ABI_VERSION ||
            candidate.structSize != sizeof(SOCFuncs) ||
            candidate.log != forwardLog || candidate.logContext != this) {
            forwardLog(this, LogLevel::Error, "SoC registration failed");
            dlclose(library);
            return;
        }

        socFuncs_ = candidate;
        socLibrary_ = library;
        socLoaded_ = true;
        forwardLog(this, LogLevel::Info, "SoC callbacks registered");
    });
    return socLoaded_;
}

void DRMPlatform::getDrmOEMContext(void** context)
{
    if (context == nullptr) return;
    *context = nullptr;
    if (socFuncs_.getDrmOEMContext != nullptr)
        socFuncs_.getDrmOEMContext(context);
    else
        defaultImpl.getDrmOEMContext(context);
}

void DRMPlatform::getDrmPlatformInitData(void** initData)
{
    if (initData == nullptr) return;
    *initData = nullptr;
    if (socFuncs_.getDrmPlatformInitData != nullptr)
        socFuncs_.getDrmPlatformInitData(initData);
    else
        defaultImpl.getDrmPlatformInitData(initData);
}

bool DRMPlatform::isAudioNeedNonSVPContext()
{
    if (socFuncs_.isAudioNeedNonSVPContext != nullptr)
        return socFuncs_.isAudioNeedNonSVPContext();
    return defaultImpl.isAudioNeedNonSVPContext();
}

bool DRMPlatform::isVideoResCheckNeed()
{
    if (socFuncs_.isVideoResCheckNeed != nullptr)
        return socFuncs_.isVideoResCheckNeed();
    return defaultImpl.isVideoResCheckNeed();
}

bool DRMPlatform::isDynamicSvpEncryptionEnabled()
{
    if (socFuncs_.isDynamicSvpEncryptionEnabled != nullptr)
        return socFuncs_.isDynamicSvpEncryptionEnabled();
    return defaultImpl.isDynamicSvpEncryptionEnabled();
}

bool DRMPlatform::loadRevocationList()
{
    if (socFuncs_.loadRevocationList != nullptr)
        return socFuncs_.loadRevocationList();
    return defaultImpl.loadRevocationList();
}

void DRMPlatform::getDrmStoragePath(std::string& readDir, std::string& storagePath,
                                   std::string& storeLocation)
{
    if (socFuncs_.getDrmStoragePath != nullptr)
        socFuncs_.getDrmStoragePath(readDir, storagePath, storeLocation);
    else
        defaultImpl.getDrmStoragePath(readDir, storagePath, storeLocation);
}

void DRMPlatform::platformInitializePlayready()
{
    if (socFuncs_.platformInitializePlayready != nullptr)
        socFuncs_.platformInitializePlayready();
    else
        defaultImpl.platformInitializePlayready();
}

void DRMPlatform::platformInitializeWidevine()
{
    if (socFuncs_.platformInitializeWidevine != nullptr)
        socFuncs_.platformInitializeWidevine();
    else
        defaultImpl.platformInitializeWidevine();
}

void DRMPlatform::platformUninitializePlayready()
{
    if (socFuncs_.platformUninitializePlayready != nullptr)
        socFuncs_.platformUninitializePlayready();
    else
        defaultImpl.platformUninitializePlayready();
}

void DRMPlatform::platformUninitializeWidevine()
{
    if (socFuncs_.platformUninitializeWidevine != nullptr)
        socFuncs_.platformUninitializeWidevine();
    else
        defaultImpl.platformUninitializeWidevine();
}

bool DRMPlatform::isSecureClockInitNeed()
{
    if (socFuncs_.isSecureClockInitNeed != nullptr)
        return socFuncs_.isSecureClockInitNeed();
    return defaultImpl.isSecureClockInitNeed();
}

} // namespace drm_platform
