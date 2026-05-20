#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

namespace cxx::demangler
{

class Demangler
{
public:
  enum class Backend : std::uint8_t
  {
    os,
    clang,
  };
  static std::size_t preallocate_buffers( const std::size_t size ) noexcept;
  std::string        operator()( const std::string_view mangled, const Backend backend = Backend::clang ) const noexcept { return call( mangled, backend ); }
  static std::string call( const std::string_view mangled, const Backend backend = Backend::clang ) noexcept;

private:
  inline static thread_local std::unique_ptr<char[]> buffer{ nullptr };
  inline static thread_local std::size_t             default_capacity{ 1024u };
  static std::string                                 call_os_backend( const std::string_view mangled );
};

}  // namespace cxx::demangler
