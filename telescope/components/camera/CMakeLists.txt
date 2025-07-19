idf_component_register(SRCS
                        Camera.cpp
                       INCLUDE_DIRS 
                        .
                       REQUIRES
# system components
                        cxx
                        log
# local components
                        arducam
                        wifi)