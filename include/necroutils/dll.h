/// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_NECROUTILS_DLL_H_
#define INCLUDE_NECROUTILS_DLL_H_

#if defined(_WIN32) || defined(__CYGWIN__)
  #if defined(NECROUTILS_LOCAL)
    #if defined(__GNUC__)
      #define NECROUTILS_API __attribute__((dllexport))
    #else
      #define NECROUTILS_API __declspec(dllexport)
    #endif
      #else
    #if defined(__GNUC__)
      #define NECROUTILS_API __attribute__((dllimport))
    #else
      #define NECROUTILS_API __declspec(dllimport)
    #endif
  #endif
  #define NECROUTILS_INTERNAL
#else
  #if __GNUC__ >= 4
    #define NECROUTILS_API __attribute__((visibility("default")))
    #define NECROUTILS_INTERNAL __attribute__((visibility("hidden")))
  #else
    #define NECROUTILS_API
    #define NECROUTILS_INTERNAL
  #endif
#endif

#endif  // INCLUDE_NECROUTILS_DLL_H_
