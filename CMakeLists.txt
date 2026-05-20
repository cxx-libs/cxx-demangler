cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(cxx-demangler VERSION 0.1.0.0 DESCRIPTION "Cross-platform C++17 demangler" HOMEPAGE_URL "https://github.com/cxx-libs/cxx-demangler" LANGUAGES CXX)

add_subdirectory(src)

add_subdirectory(examples)
