#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace config
{

namespace build
{

constexpr const char* directory() { return "/home/stud/s3443408/Projects/rmit-rendering-2/portal"; }
constexpr const char* type() { return "Debug"; }
constexpr const char* system() { return "Linux-3.2.9-2.fc16.x86_64"; }
constexpr const char* systemName() { return "Linux"; }
constexpr const char* compilerVersion() { return "/usr/lib64/ccache/c++"; }
constexpr const char* compilerFlags() { return "-Wall -W -Werror -Wextra -std=gnu++0x"; }

constexpr const char* version() { return "v.0.1"; }
constexpr int buildNumber() { return 1000; }

}

}

#endif /* CONFIG_HPP */
