cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Debug)

project (nix-password-checker_bin)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-W -Wall -Wextra")

include(CheckIncludeFileCXX)
check_include_file_cxx(libconfig.h++ HAVE_LIBCONFIG_HPP)
if(NOT HAVE_LIBCONFIG_HPP)
  message( FATAL_ERROR "libconfig not found")
endif()

set(nix-password-checker_source_files main.cpp password_checker.cpp)
add_executable(nix-password-checker ${nix-password-checker_source_files})

target_link_libraries(nix-password-checker config++ crypt)
