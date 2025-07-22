idf_component_register(SRCS
                        ControlCameraCmd.cpp
                        MoveBaseServosCmd.cpp
                        PlanTrajectoryCmd.cpp
                        TelemRateCmd.cpp
                        FocusCmd.cpp
                        OTAUpdateCmd.cpp
                       INCLUDE_DIRS 
                        .
                       REQUIRES
# system components
                        cxx
# local components
                        utils)