#include "cxx/demangler/demangler.hpp"

#include "Demangle.h"

#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>

#if defined( _WIN32 )

  #include <mutex>
// clang-format off
  #include <windows.h>
  #include <dbghelp.h>
// clang-format on

  #pragma comment( lib, "Dbghelp.lib" )

std::string cxx::demangler::Demangler::call_os_backend( const std::string_view mangled )
{
  if( mangled.empty() ) return {};
  if( mangled.front() != '?' ) return std::string( mangled );  // MSVC symbols usually start with '?'
  if( !buffer ) buffer.reset( new( std::nothrow ) char[default_capacity] );
  if( !buffer ) return std::string( mangled );
  // DbgHelp is globally non-thread-safe
  static std::mutex mutex;

  DWORD result = 0;
  DWORD size   = default_capacity;
  do
  {
    {
      std::lock_guard lock( mutex );
      result = ::UnDecorateSymbolName( std::string( mangled ).data(), buffer.get(), static_cast<DWORD>( size ), UNDNAME_COMPLETE );
    }

    if( result != 0 ) return { buffer.get(), static_cast<std::size_t>( result ) };
    size *= 2;
    buffer.reset( new( std::nothrow ) char[size] );
  } while( result == 0 && size <= max_capacity );
  return std::string( mangled );
}

#else

  #include <cxxabi.h>

std::string cxx::demangler::Demangler::call_os_backend( const std::string_view mangled )
{
  if( !buffer ) buffer.reset( new( std::nothrow ) char[default_capacity] );
  if( mangled.empty() ) return {};
  if( buffer )
  {
    static thread_local int   status{ 0 };
    static thread_local char* result{ nullptr };
    result = ::abi::__cxa_demangle( std::string( mangled ).data(), buffer.get(), &default_capacity, &status );
    if( status == 0 && result )
    {
      if( result != buffer.get() )
      {
        // __cxa_demangle allocated a new buffer.
        // Old buffer was freed internally, so we just adopt the new one.
        buffer.release();        // release ownership of the (already freed) old pointer
        buffer.reset( result );  // now buffer owns the new pointer
      }
      return std::string( result );
    }
    else
      return std::string( mangled );  // fallback
  }
  else
    return std::string( mangled );  // fallback
}

#endif

std::size_t cxx::demangler::Demangler::preallocate_buffers( std::size_t size ) noexcept
{
  if( !buffer || size > default_capacity )
  {
    buffer.reset( new( std::nothrow ) char[size] );
    if( buffer ) default_capacity = size;
    else
      return 0;
  }
  return default_capacity;
}

std::string cxx::demangler::Demangler::call( const std::string_view mangled, cxx::demangler::Demangler::Backend backend ) noexcept
{
  switch( backend )
  {
    case cxx::demangler::Demangler::Backend::os:
    {
      return call_os_backend( mangled );
    }
    case cxx::demangler::Demangler::Backend::clang:
    {
      return cxx::demangler::backend::clang::demangle( mangled );
    }
    default: return std::string( mangled );
  }
}
