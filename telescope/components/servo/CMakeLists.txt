idf_component_register(SRCS 
                        CustomServo.cpp
                        PIDController.cpp
                       INCLUDE_DIRS 
                        .
                       REQUIRES
# system components
                        cxx
                        log
# local components
                        arduino
                        esp32servo)