#ifndef __UTILS_H__
#define __UTILS_H__

#include <Arduino.h>

#define FOREVER for(;;)

#ifdef DEBUG

  #define STRINGIFY(x) #x
  #define TOSTRING(x) STRINGIFY(x)
  #define AT __FILE__ " (" TOSTRING(__LINE__) "): "

  #define DEBUG_INIT \
            Serial.begin(9600); \
            while (!Serial) { ; } \
            Serial.println("I'm awake!");

  #define DEBUG_PRINT(str) Serial.print(AT str)
  #define DEBUG_PRINTLN(str) Serial.println(AT str)

  #ifdef DEBUG_ENTER_EXIT
    #define DEBUG_ENTER(str) DEBUG_PRINTLN("Entering: " str)
    #define DEBUG_EXIT(str) DEBUG_PRINTLN("Exiting: " str)
  #else
    #define DEBUG_ENTER(str) (void)0
    #define DEBUG_EXIT(str) (void)0
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
  
  #define DEBUG_HEARTBEAT(str) (void)0

#endif 

#endif