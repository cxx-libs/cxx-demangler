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
  static std::size_t preallocate_buffers( std::size_t size ) noexcept;
  std::string        operator()( std::string_view mangled, const Backend backend = Backend::os ) const noexcept { return call( mangled, backend ); }
  static std::string call( const std::string_view mangled, const Backend backend = Backend::os ) noexcept;

private:
  inline static thread_local std::unique_ptr<char[]> buffer{ nullptr };
  inline static thread_local std::size_t             default_capacity{ 1024u };
};

}  // namespace cxx::demangler
