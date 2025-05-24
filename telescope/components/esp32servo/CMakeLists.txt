idf_component_register(SRCS 
                        ESP32Servo/src/ESP32PWM.cpp
                        ESP32Servo/src/ESP32Servo.cpp
                       INCLUDE_DIRS 
                        ESP32Servo/src
                       REQUIRES
# system components
                        cxx
                        esp_timer
                        log
# local components
                        arduino)