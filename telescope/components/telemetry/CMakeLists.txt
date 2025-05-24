idf_component_register(SRCS 
                        "Telemetry.cpp"
                       INCLUDE_DIRS 
                        "."
                        "../serializer"
                       REQUIRES
                        "serializer")