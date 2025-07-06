#ifndef __WIFI_INITIALIZER_H__
#define __WIFI_INITIALIZER_H__

#include <mutex>

class WifiInitializer
{
public:
    static void initialize();

    ~WifiInitializer();

    WifiInitializer(const WifiInitializer&) = delete;
    WifiInitializer(WifiInitializer&&) = delete;
    WifiInitializer& operator=(const WifiInitializer&) = delete;
    WifiInitializer& operator=(WifiInitializer&&) = delete;

private:
    WifiInitializer() = default;
    static void initWifiOnce();

    static std::once_flag initFlag; ///< Flag to ensure WiFi is initialized only once
};

#endif // __WIFI_INITIALIZER_H__