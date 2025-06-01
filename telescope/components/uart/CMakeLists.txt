idf_component_register(SRCS
                        Uart.cpp
                       INCLUDE_DIRS 
                        .
                       REQUIRES
# system components
                        cxx
                        esp_driver_gpio
                        esp_driver_uart)