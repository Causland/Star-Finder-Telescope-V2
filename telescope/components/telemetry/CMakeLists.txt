idf_component_register(SRCS 
                        Telemetry.cpp
                       INCLUDE_DIRS 
                        .
                       REQUIRES
# system components
                        cxx
                        esp_timer
                        freertos
                        log
# local components
                        utils)