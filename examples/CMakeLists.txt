add_executable(demangle.example demangle.cpp)
target_link_libraries(demangle.example PRIVATE cxx::demangler::static)
