
#include "base/WindowsVersion.h"
#include <windows.h>


/////////////////////////////////////////////////
// 临时找的这个地方，用来做三方lib库的链接，避免通过项目属性来设置，
// 如有更好的地方，请转移。
/////////////////////////////////////////////////
#ifdef _WIN32
#ifdef _WIN64
//#pragma comment(lib, "../../3rdlib/libcurl_a.lib")
#else
#pragma comment(lib, "../../3rdlib/libcurl_a.lib")
#endif // _WIN64 
#endif // _WIN32
/////////////////////////////////////////////////



namespace base {

WindowsVersion getWindowsVersion(int* major, int* minor)
{
    static bool initialized = false;
    static WindowsVersion version;
    static int majorVersion, minorVersion;

    if (initialized)
        return version;
    initialized = true;

    OSVERSIONINFOEX versionInfo = { 0 };
    versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
    GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&versionInfo));
    majorVersion = versionInfo.dwMajorVersion;
    minorVersion = versionInfo.dwMinorVersion;

    if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32s)
        version = Windows3_1;
    else if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
        if (!minorVersion)
            version = Windows95;
        else
            version = (minorVersion == 10) ? Windows98 : WindowsME;
    } else {
        if (majorVersion == 5) {
            if (!minorVersion)
                version = Windows2000;
            else
                version = (minorVersion == 1) ? WindowsXP : WindowsServer2003;
        } else if (majorVersion >= 6) {
            if (minorVersion >= 2)
                version = Windows8;
            else if (versionInfo.wProductType == VER_NT_WORKSTATION)
                version = (majorVersion == 6 && !minorVersion) ? WindowsVista : Windows7;
            else
                version = WindowsServer2008;
        } else
            version = (majorVersion == 4) ? WindowsNT4 : WindowsNT3;
    }

    if (major)
        *major = majorVersion;
    if (minor)
        *minor = minorVersion;
    return version;
}

}