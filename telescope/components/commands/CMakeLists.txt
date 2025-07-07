idf_component_register(SRCS
                        ControlCameraCmd.cpp
                        MoveServoCmd.cpp
                        PlanTrajectoryCmd.cpp
                        TelemRateCmd.cpp
                       INCLUDE_DIRS 
                        .
                       REQUIRES
# system components
                        cxx
# local components
                        serializer)