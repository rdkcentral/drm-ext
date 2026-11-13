/* Copyright 2026 RDK Management. SPDX-License-Identifier: Apache-2.0 */
#ifndef DRM_PLATFORM_DEFAULT_DRM_PLATFORM_H
#define DRM_PLATFORM_DEFAULT_DRM_PLATFORM_H

#include <string>

namespace drm_platform {

/* Stub policy. Review these defaults against each product's DRM requirements. */
class DefaultDRMPlatform {
public:
    void getDrmOEMContext(void** context) const { *context = nullptr; }
    void getDrmPlatformInitData(void** initData) const { *initData = nullptr; }
    bool isAudioNeedNonSVPContext() const { return false; }
    bool isVideoResCheckNeed() const { return false; }
    bool isDynamicSvpEncryptionEnabled() const { return false; }
    bool loadRevocationList() const { return false; }
    void getDrmStoragePath(std::string& readDir, std::string& storagePath,
                           std::string& storeLocation) const
    {
        readDir.clear();
        storagePath.clear();
        storeLocation.clear();
    }
    void platformInitializePlayready() const {}
    void platformInitializeWidevine() const {}
    void platformUninitializePlayready() const {}
    void platformUninitializeWidevine() const {}
    bool isSecureClockInitNeed() const { return false; }
};

} // namespace drm_platform
#endif
