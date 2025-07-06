idf_component_register(SRCS 
                        UDPReceiver.cpp
                        UDPSender.cpp
                        WifiInitializer.cpp
                       INCLUDE_DIRS 
                        .
                       REQUIRES
# system components
                        cxx
                        log
                        esp_wifi)