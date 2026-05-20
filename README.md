# CXX Demangler

A lightweight, thread-safe, modern C++ symbol demangling library with support for both OS-native and Clang-based demangling backends.

Designed for tooling, debugging utilities, profilers, crash reporters, logging systems, and runtime introspection.

---

## Features

- Modern C++ API (`std::string_view`, `std::string`)
- Thread-safe design
- Thread-local reusable buffers
- Configurable buffer preallocation
- Cross-platform support
  - Windows (`DbgHelp`)
  - Linux / macOS (`abi::__cxa_demangle`)
- Optional Clang-based backend
- Graceful fallback behavior
- Zero exceptions (`noexcept` API)
- Lightweight and dependency-friendly

---

## Supported Backends

| Backend | Description |
|---|---|
| `Backend::os` | Native platform demangler |
| `Backend::clang` | Clang demangling implementation |

### Platform Details

#### Windows

Uses:

- `DbgHelp::UnDecorateSymbolName`

Supports MSVC mangled symbols.

#### Linux / macOS

Uses:

- `abi::__cxa_demangle`

Supports Itanium ABI mangled symbols produced by GCC and Clang.

---

# Installation

## CMake

```cmake
add_subdirectory(cxx-demangler)

target_link_libraries(your_target PRIVATE cxx-demangler)
```

---

# Basic Usage

```cpp
#include <iostream>
#include "cxx/demangler/demangler.hpp"

int main()
{
    cxx::demangler::Demangler demangler;

    std::string result = demangler("_ZN3foo3barEv", cxx::demangler::Demangler::Backend::clang);

    std::cout << result << '\n';
}
```

---

# Static API

You may also use the static interface directly:

```cpp
std::string result = cxx::demangler::Demangler::call("_ZN3foo3barEv", cxx::demangler::Demangler::Backend::os);
```

---

# Available Backends

```cpp
enum class Backend : std::uint8_t
{
    os,
    clang,
};
```

## `Backend::os`

Uses the operating system native demangler.

### Windows

```text
?func@@YAXXZ
```

↓

```cpp
void __cdecl func(void)
```

### GCC / Clang (Itanium ABI)

```text
_ZN3foo3barEv
```

↓

```cpp
foo::bar()
```

---

## `Backend::clang`

Uses the bundled Clang demangling implementation.

Useful when:

- Consistent output is desired across platforms
- You need support for both MSVC and Itanium ABI names
- You want deterministic formatting

---

# Thread Safety

The library is fully thread-safe.

Internally it uses:

- `thread_local` reusable buffers
- Mutex protection around Windows `DbgHelp` calls

Each thread owns its own demangling buffer to minimize allocations and synchronization overhead.

---

# Buffer Management

## Preallocate Buffers

For hot paths or large-scale demangling workloads, buffers can be preallocated:

```cpp
cxx::demangler::Demangler::preallocate_buffers(1024 * 64);
```

This reduces repeated heap allocations during demangling.

---

## Set Maximum Buffer Size

```cpp
cxx::demangler::Demangler::set_max_size(1024 * 1024);
```

Default maximum buffer size:

```text
1 MiB
```

This prevents pathological symbol names from causing unbounded memory growth.

---

# Failure Behavior

The library never throws exceptions.

If demangling fails:

- The original mangled symbol is returned unchanged
- Empty input returns an empty string

Example:

```cpp
std::string result = cxx::demangler::Demangler::call("not_a_symbol");

// result == "not_a_symbol"
```

---

# Example

```cpp
#include <iostream>
#include "cxx/demangler/demangler.hpp"

int main()
{
    using Backend = cxx::demangler::Demangler::Backend;

    cxx::demangler::Demangler demangler;

    std::cout << demangler("_ZN3foo3barEv", Backend::clang) << '\n';

    std::cout << demangler("?func@@YAXXZ", Backend::os) << '\n';
}
```

---

# Design Goals

- Minimal overhead
- Predictable behavior
- No exception propagation
- Reusable allocations
- Cross-platform consistency
- Tooling-friendly API

---

# Requirements

- C++17 or newer
- Clang / GCC / MSVC

---

# License

MIT License

---

# Contributing

Contributions, bug reports, and feature requests are welcome.

Please open an issue or submit a pull request.

---

# Notes

### Windows

Link against:

```text
DbgHelp.lib
```

Already handled internally through:

```cpp
#pragma comment(lib, "Dbghelp.lib")
```

for MSVC builds.

---
