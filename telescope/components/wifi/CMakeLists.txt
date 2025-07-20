idf_component_register(SRCS 
                        UDPReceiver.cpp
                        UDPSender.cpp
                        WifiInitializer.cpp
                       INCLUDE_DIRS 
                        .
                       REQUIRES
# system components
                        cxx
                        esp_event
                        esp_wifi
                        log)