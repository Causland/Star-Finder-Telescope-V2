idf_component_register(SRCS 
                        "GPS.cpp"
                       INCLUDE_DIRS 
                        "."
                        "../tinygpsplus/TinyGPSPlus/src"
                       REQUIRES
                        "arduino"
                        "tinygpsplus")