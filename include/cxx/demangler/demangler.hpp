#pragma once
#include "cxx/demangler/export.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>

namespace cxx::demangler
{

/**
 * @brief A C++ symbol name demangler that converts mangled names to human-readable format.
 *
 * The Demangler class provides functionality to convert mangled C++ symbol names (as produced by
 * various C++ compilers) into their human-readable format. It supports multiple demangling backends:
 * - OS-native backends (e.g., Windows DbgHelp, GCC libstdc++ abi)
 * - Clang-based demangling
 *
 * @details
 * The class uses thread-local buffering for efficient memory management and supports pre-allocation
 * of demangling buffers for performance optimization. It handles platform-specific demangling
 * differences transparently.
 *
 * Example usage:
 * @code
 * cxx::demangler::Demangler demangler;
 * std::string demangled = demangler("_ZN3fooEv", cxx::demangler::Demangler::Backend::clang);
 * // Or using the call method:
 * demangled = cxx::demangler::Demangler::call("_ZN3fooEv", cxx::demangler::Demangler::Backend::clang);
 * @endcode
 *
 * @thread_safety Thread-safe. Uses thread-local buffers and mutex protection for OS backends.
 */
class Demangler
{
public:
  /**
   * @brief Available demangling backends.
   *
   * Specifies which demangling implementation to use. The choice affects:
   * - Symbol format compatibility
   * - Demangling behavior and output format
   * - Performance characteristics
   */
  enum class Backend : unsigned char
  {
    /**
     * @brief Use the operating system's native demangling backend.
     *
     * On Windows: Uses DbgHelp::UnDecorateSymbolName for MSVC mangled names.
     * On POSIX systems: Uses libstdc++'s abi::__cxa_demangle for GCC/Clang names.
     *
     * @note MSVC symbols typically start with '?'. Other formats are returned as-is.
     */
    os,

    /**
     * @brief Use Clang-based demangling.
     *
     * Provides Clang's demangling implementation. This is the default backend
     * and works with Itanium ABI and MSVC mangled names.
     */
    clang,
  };

  /**
   * @brief Pre-allocates internal buffers for demangling operations.
   *
   * Allocates thread-local buffers to the specified size for better performance
   * when demangling many symbols. This method is useful to avoid repeated allocations
   * in hot code paths.
   *
   * @param size The size in bytes to allocate. If larger than current capacity,
   *             a new buffer of this size is allocated.
   *
   * @return The actual allocated capacity in bytes, or 0 if allocation fails.
   *
   * @note This operation is per-thread. Each thread has its own buffer.
   *
   * @thread_safety Safe to call from any thread.
   *
   * @see set_max_size
   */
  CXX_EXPORT static std::size_t preallocate_buffers( const std::size_t size ) noexcept;

  /**
   * @brief Sets the maximum allowed buffer size.
   *
   * Configures the maximum size that buffers can grow to during demangling operations.
   * This prevents unbounded memory allocation in case of pathologically large symbol names.
   *
   * @param size The maximum buffer size in bytes. Default is 1MB (1024 * 1024).
   *
   * @note This is a global setting shared across all threads.
   *
   * @thread_safety Safe to call, but changes affect all threads.
   *
   * @see preallocate_buffers
   */
  CXX_EXPORT static void set_max_size( const std::size_t size ) noexcept { max_capacity = size; }

  /**
   * @brief Function call operator for demangling.
   *
   * Provides a convenient operator() interface for demangling symbols.
   * Equivalent to calling the static @c call method.
   *
   * @param mangled The mangled symbol name to demangle.
   * @param backend The demangling backend to use (default: Backend::clang).
   *
   * @return The demangled symbol name. If demangling fails or the symbol is not
   *         recognized by the backend, returns the original mangled name.
   *
   * @thread_safety Safe to call from any thread.
   *
   * @see call
   */
  CXX_EXPORT std::string operator()( const std::string_view mangled, const Backend backend = Backend::clang ) const noexcept { return call( mangled, backend ); }

  /**
   * @brief Demangles a symbol name using the specified backend.
   *
   * Converts a mangled C++ symbol name into human-readable format using the
   * specified demangling backend.
   *
   * @param mangled The mangled symbol name to demangle. Can be empty.
   * @param backend The demangling backend to use (default: Backend::clang).
   *
   * @return The demangled symbol name in human-readable format.
   *         - If demangling succeeds: the demangled name
   *         - If demangling fails: the original @c mangled name is returned
   *         - If @c mangled is empty: returns an empty string
   *
   * @note On Windows with Backend::os, symbols not starting with '?' are returned as-is.
   *
   * @thread_safety Safe to call from any thread, including concurrent calls.
   *
   * @see Backend
   * @see preallocate_buffers
   */
  CXX_EXPORT static std::string call( const std::string_view mangled, const Backend backend = Backend::clang ) noexcept;

private:
  /**
   * @brief Thread-local buffer for demangling operations.
   *
   * Each thread has its own buffer to avoid synchronization overhead
   * during demangling. The buffer is allocated on-demand and reused.
   */
  inline static thread_local std::unique_ptr<char[]> buffer{ nullptr };

  /**
   * @brief Initial and current default buffer capacity.
   *
   * The default size for newly allocated buffers. Updated by @c preallocate_buffers
   * when a larger buffer is needed.
   */
  inline static std::size_t default_capacity{ 1024u };

  /**
   * @brief Maximum allowed buffer capacity.
   *
   * Prevents unbounded growth of demangling buffers. Default is 1MB.
   * Can be configured via @c set_max_size.
   */
  inline static std::size_t max_capacity{ 1024 * 1024 };

  /**
   * @brief Calls the operating system's native demangling backend.
   *
   * Internal method that implements platform-specific demangling:
   * - On Windows: Uses DbgHelp::UnDecorateSymbolName
   * - On POSIX: Uses abi::__cxa_demangle
   *
   * @param mangled The mangled symbol name to demangle.
   *
   * @return The demangled name, or the original mangled name if demangling fails.
   *
   * @thread_safety Safe but uses mutex protection on Windows due to DbgHelp limitations.
   *
   * @internal
   */
  static std::string call_os_backend( const std::string_view mangled );
};

}  // namespace cxx::demangler
