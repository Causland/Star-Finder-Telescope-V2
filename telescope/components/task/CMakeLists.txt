idf_component_register(SRCS 
                        CustomTask.cpp
                       INCLUDE_DIRS 
                        .
                       REQUIRES
# system components
                        cxx
                        esp_timer
                        log
                        pthread
# local components
                        commands
                        telemetry)