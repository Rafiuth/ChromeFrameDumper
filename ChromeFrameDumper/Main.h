#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <filesystem>

#include <windows.h>
#include <MinHook.h>

#include "Logger.h"
#include "MediaInterfaces.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>

//as if vcpkg didn't already suck enough, we need this.
#pragma comment(lib, "Bcrypt.lib")
#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfuuid.lib")
#pragma comment(lib, "Strmiids.lib")
}

namespace fs = std::filesystem;
using namespace std::chrono;

#define DETOUR_FUNC(retType, name,  args)       \
    typedef retType (*name##_FuncType)args;     \
    name##_FuncType name##_Orig;                \
    retType name##_Detour##args

#define CREATE_HOOK(addr, funcName)             \
    MH_StatusCheck(MH_CreateHook((LPVOID)(addr), &funcName##_Detour, (LPVOID*)&funcName##_Orig), "Failed to create hook " #funcName)

inline void MH_StatusCheck(MH_STATUS status, const char* msg)
{
    if (status != MH_OK) {
        throw std::runtime_error(std::string(msg) + ": " + MH_StatusToString(status));
    }
}