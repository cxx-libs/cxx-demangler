#pragma once

#if defined( _WIN32 ) || defined( __CYGWIN__ )
  #if defined( CXX_SHARED )
    #ifdef CXX_EXPORTS
      #define CXX_EXPORT __declspec( dllexport )
    #else
      #define CXX_EXPORT __declspec( dllimport )
    #endif
  #else
    #define CXX_EXPORT
  #endif
  #define CXX_HIDDEN
#else
  #define CXX_EXPORT __attribute__( ( visibility( "default" ) ) )
  #define CXX_HIDDEN __attribute__( ( visibility( "hidden" ) ) )
#endif
