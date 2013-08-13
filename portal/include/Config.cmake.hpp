#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace config
{

namespace build
{

constexpr const char* directory() { return "@PROJECT_SOURCE_DIR@"; }
constexpr const char* type() { return "@CMAKE_BUILD_TYPE@"; }
constexpr const char* system() { return "@CMAKE_SYSTEM@"; }
constexpr const char* systemName() { return "@CMAKE_SYSTEM_NAME@"; }
constexpr const char* compilerVersion() { return "@CMAKE_CXX_COMPILER@"; }
constexpr const char* compilerFlags() { return "@CMAKE_CXX_FLAGS@"; }

}

}

#endif /* CONFIG_HPP */