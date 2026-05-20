#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>

namespace cxx::demangler
{

class Demangler
{
public:
  static std::size_t preallocate_buffers( std::size_t size ) noexcept;
  std::string        operator()( std::string_view mangled, bool dumb = true ) const noexcept { return call( mangled ); }
  static std::string call( const std::string_view mangled ) noexcept;

private:
  inline static thread_local std::unique_ptr<char[]> buffer{ nullptr };
  inline static thread_local std::size_t             default_capacity{ 1024u };
};

}  // namespace cxx::demangler
