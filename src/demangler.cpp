#include "cxx/demangler/demangler.hpp"

#include <cstddef>
#include <cstring>
#include <cxxabi.h>
#include <string>
#include <string_view>

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

std::string cxx::demangler::Demangler::call( const std::string_view mangled ) noexcept
{
  if( !buffer ) buffer.reset( new( std::nothrow ) char[default_capacity] );
  if( buffer )
  {
    static thread_local int   status{ 0 };
    static thread_local char* result{ nullptr };
    result = ::abi::__cxa_demangle( mangled.data(), buffer.get(), &default_capacity, &status );
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
