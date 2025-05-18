#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdio>

#define FOREVER for(;;)

#ifdef DEBUG

  #define STRINGIFY(x) #x
  #define TOSTRING(x) STRINGIFY(x)
  #define AT __FILE__ " (" TOSTRING(__LINE__) "): "

  #define DEBUG_INIT \
            printf("I'm awake!\n"); fflush(stdout);

  #define DEBUG_PRINT(str) printf(AT str); fflush(stdout);
  #define DEBUG_PRINTLN(str) printf(AT str##"\n"); fflush(stdout);

  #ifdef DEBUG_ENTER_EXIT
    #define DEBUG_ENTER(str) DEBUG_PRINTLN("Entering: " str) 
    #define DEBUG_EXIT(str) DEBUG_PRINTLN("Exiting: " str)
  #else
    #define DEBUG_ENTER(str) (void)0
    #define DEBUG_EXIT(str) (void)0
  #endif

  #ifdef DEBUG_TRAJECTORY
    #define DEBUG_TRAJECTORY_PRINT(str) DEBUG_PRINTLN(str);
  #else
    #define DEBUG_TRAJECTORY_PRINT(str) (void)0
  #endif

  #ifdef DEBUG_TELEMETRY
    #define DEBUG_TELEMETRY_PRINT(str) DEBUG_PRINTLN(str);
  #else
    #define DEBUG_TELEMETRY_PRINT(str) (void)0
  #endif

  #ifdef DEBUG_CAMERA
    #define DEBUG_CAMERA_PRINT(str) DEBUG_PRINTLN(str);
  #else
    #define DEBUG_CAMERA_PRINT(str) (void)0
  #endif

  #ifdef DEBUG_FOREVER_HB
    #define DEBUG_HEARTBEAT(str) DEBUG_PRINTLN("Heartbeat: " str);
  #else
    #define DEBUG_HEARTBEAT(str) (void)0
  #endif

#else

  #define DEBUG_INIT (void)0

  #define DEBUG_PRINT(str) (void)0
  #define DEBUG_PRINTLN(str) (void)0

  #define DEBUG_ENTER(str) (void)0
  #define DEBUG_EXIT(str) (void)0

  #define DEBUG_TRAJECTORY_PRINT(str) (void)0

  #define DEBUG_TELEMETRY_PRINT(str) (void)0

  #define DEBUG_CAMERA_PRINT(str) (void)0
  
  #define DEBUG_HEARTBEAT(str) (void)0

#endif 

#endif