add_library(cxx_demangler SHARED
    demangler.cpp
    clang/Demangle.cpp
    clang/cxa_demangle.cpp
    clang/ItaniumDemangle.cpp
    clang/MicrosoftDemangle.cpp
    clang/MicrosoftDemangleNodes.cpp
)

target_include_directories(cxx_demangler
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/clang>
        $<INSTALL_INTERFACE:include/cxx/demangler>
)

set_target_properties(cxx_demangler PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
    CXX_STANDARD 23
    PUBLIC_HEADER "include/cxx/demangler/demangler.hpp"
)
#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.4)
#    message(FATAL_ERROR "You are on an extremely old version of GCC. Please update your compiler to at least GCC 5.4, preferably latest")
#  elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
#    message(WARNING "Old Version of GCC detected. Using Legacy C++ support")
#    target_link_libraries(FlakedTuna PRIVATE -lstdc++fs)
#    target_compile_definitions(FlakedTuna PUBLIC LEGACY_CXX)
#  endif()
#endif()

add_library(cxx::demangler ALIAS cxx_demangler)

install(TARGETS cxx_demangler EXPORT cxx_demangler RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION ${PUBLIC_INCLUDE_INSTALL_DIR})

install(EXPORT cxx_demangler FILE "demangler.cmake" DESTINATION "lib/cmake/cxx/demangler" NAMESPACE cxx::)
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/generated/demanglerConfigVersion.cmake" VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
configure_package_config_file("${PROJECT_SOURCE_DIR}/cmake/demanglerConfig.cmake.in" "${PROJECT_BINARY_DIR}/generated/demanglerConfig.cmake" INSTALL_DESTINATION "lib/cmake/cxx/demangler")
install(FILES "${PROJECT_BINARY_DIR}/generated/demanglerConfig.cmake" "${PROJECT_BINARY_DIR}/generated/demanglerConfigVersion.cmake" DESTINATION "lib/cmake/cxx/demangler")
