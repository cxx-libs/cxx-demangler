cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(cxx-demangler VERSION 0.1.0.0 DESCRIPTION "Cross-platform C++17 demangler" HOMEPAGE_URL "https://github.com/cxx-libs/cxx-demangler" LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(src)

add_subdirectory(examples)
