#include "drm_platform/DRMPlatform.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace {
void require(bool condition, const char* message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

void captureLog(void* context, drm_platform::LogLevel, const char* message)
{
    require(context != nullptr, "log context reaches common sink");
    require(message != nullptr, "log message is non-null");
    static_cast<std::vector<std::string>*>(context)->emplace_back(message);
}
} // namespace

int main(int argc, char** argv)
{
    require(argc == 2, "expected mode");
    const bool expectSoc = std::string(argv[1]) == "soc";
    // Remains alive through the singleton's lifetime.
    static std::vector<std::string> logs;
    auto& platform = drm_platform::DRMPlatform::instance();

    require(!platform.isDynamicSvpEncryptionEnabled(), "defaults before init");
    require(platform.init(captureLog, &logs) == expectSoc, "registration result");
    const auto logCount = logs.size();
    require(logCount != 0, "loader or SoC logs use common sink");
    require(platform.init() == expectSoc, "init result is stable");
    require(logs.size() == logCount, "second init does not register again");

    require(platform.isDynamicSvpEncryptionEnabled() == expectSoc,
            "SoC override or default");
    if (expectSoc) {
        require(logs.back() == "example dynamic SVP query",
                "SoC retained common logging callback and context");
    }

    require(!platform.isVideoResCheckNeed(), "false callback result is respected");
    if (expectSoc) {
        require(logs.back() == "example resolution query", "false callback ran");
    }

    // Unset callbacks keep their defaults even in a successfully loaded module.
    require(!platform.isAudioNeedNonSVPContext(), "unset audio default");
    require(!platform.isSecureClockInitNeed(), "unset clock default");
    require(!platform.loadRevocationList(), "unset revocation default");

    int marker = 0;
    void* context = &marker;
    platform.getDrmOEMContext(&context);
    require(context == nullptr, "unset OEM output is cleared");
    context = &marker;
    platform.getDrmPlatformInitData(&context);
    require(context == nullptr, "unset init-data output is cleared");
    platform.getDrmOEMContext(nullptr);
    platform.getDrmPlatformInitData(nullptr);

    std::string readDir = "stale", storagePath = "stale", storeLocation = "stale";
    platform.getDrmStoragePath(readDir, storagePath, storeLocation);
    require(readDir.empty() && storagePath.empty() && storeLocation.empty(),
            "unset storage outputs are cleared");

    platform.platformInitializePlayready();
    platform.platformInitializeWidevine();
    platform.platformUninitializePlayready();
    platform.platformUninitializeWidevine();
    std::cout << "DRMPlatform " << argv[1] << " checks passed\n";
}
