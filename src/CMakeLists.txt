include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# ============================================================
# Sources
# ============================================================

set(CXX_DEMANGLER_SOURCES
    demangler.cpp
    clang/Demangle.cpp
    clang/ItaniumDemangle.cpp
    clang/MicrosoftDemangle.cpp
    clang/MicrosoftDemangleNodes.cpp
)

# ============================================================
# Shared Library
# ============================================================

add_library(cxx_demangler_shared SHARED ${CXX_DEMANGLER_SOURCES})

add_library(cxx::demangler::shared ALIAS cxx_demangler_shared)

target_compile_features(cxx_demangler_shared PUBLIC cxx_std_17)

target_include_directories(cxx_demangler_shared
                            PUBLIC
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/clang>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_definitions(cxx_demangler_shared PUBLIC CXX_EXPORTS)
set_target_properties(cxx_demangler_shared
                      PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        CXX_EXTENSIONS OFF
                        CXX_STANDARD_REQUIRED ON
                        CXX_STANDARD 17
                        OUTPUT_NAME cxx_demangler
                        DEBUG_POSTFIX d
                        CXX_VISIBILITY_PRESET hidden
                        VISIBILITY_INLINES_HIDDEN YES
                        PUBLIC_HEADER
                          "${PROJECT_SOURCE_DIR}/include/cxx/demangler/demangler.hpp;${PROJECT_SOURCE_DIR}/include/cxx/demangler/export.hpp"
                        VERSION ${PROJECT_VERSION}
                        SOVERSION ${PROJECT_VERSION_MAJOR})

# ============================================================
# Static Library
# ============================================================

add_library(cxx_demangler_static STATIC ${CXX_DEMANGLER_SOURCES})

add_library(cxx::demangler::static ALIAS cxx_demangler_static)

target_compile_features(cxx_demangler_static PUBLIC cxx_std_17)

target_include_directories(cxx_demangler_static
                            PUBLIC
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/clang>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(cxx_demangler_static
                      PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        CXX_EXTENSIONS OFF
                        CXX_STANDARD_REQUIRED ON
                        CXX_STANDARD 17
                        OUTPUT_NAME cxx_demangler
                        PUBLIC_HEADER
                          "${PROJECT_SOURCE_DIR}/include/cxx/demangler/demangler.hpp;${PROJECT_SOURCE_DIR}/include/cxx/demangler/export.hpp"
                        DEBUG_POSTFIX d)

# ============================================================
# Linux debug symbols
# ============================================================

if(UNIX AND NOT APPLE)
  add_custom_command(TARGET cxx_demangler_shared
                      POST_BUILD
                      COMMENT "Linux debug symbols"
                      COMMAND ${CMAKE_OBJCOPY} --only-keep-debug $<TARGET_FILE:cxx_demangler_shared> $<TARGET_FILE:cxx_demangler_shared>.debug
                      COMMAND ${CMAKE_STRIP} --strip-debug $<TARGET_FILE:cxx_demangler_shared>
                      COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink=$<TARGET_FILE:cxx_demangler_shared>.debug $<TARGET_FILE:cxx_demangler_shared>)
  install(FILES $<TARGET_FILE:cxx_demangler_shared>.debug
          DESTINATION "${CMAKE_INSTALL_LIBDIR}/cxx/demangler/debug")
endif()

# ============================================================
# Install Targets
# ============================================================

install(TARGETS
        cxx_demangler_shared
        cxx_demangler_static
        EXPORT cxx_demangler_targets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/cxx/demangler"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/cxx/demangler"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/cxx/demangler"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/cxx/demangler")

# ============================================================
# Export Targets
# ============================================================

install(EXPORT cxx_demangler_targets
        FILE cxx_demanglerTargets.cmake
        NAMESPACE cxx::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cxx_demangler")

# ============================================================
# Package Config
# ============================================================

configure_package_config_file("${PROJECT_SOURCE_DIR}/cmake/cxx_demanglerConfig.cmake.in" "${PROJECT_BINARY_DIR}/generated/cxx_demanglerConfig.cmake"
                              INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cxx_demangler")

write_basic_package_version_file("${PROJECT_BINARY_DIR}/generated/cxx_demanglerConfigVersion.cmake"
                                  VERSION ${PROJECT_VERSION}
                                  COMPATIBILITY SameMajorVersion)

install(FILES "${PROJECT_BINARY_DIR}/generated/cxx_demanglerConfig.cmake" "${PROJECT_BINARY_DIR}/generated/cxx_demanglerConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cxx_demangler")
