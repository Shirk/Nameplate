cmake_minimum_required(VERSION 3.28)

project(Nameplate
    VERSION 0.51
    LANGUAGES CXX
    DESCRIPTION "Corrects defects in nameplate rendering."
    HOMEPAGE_URL "https://www.github.com/Shirk/Nameplate"
)

# set(ASHITA_SDK_PATH "../Ashita v4 SDK 4.11/plugins/sdk" CACHE STRING "Ashita SDK Path")
# set(ASHITA_SDK_PATH "../Ashita v4 SDK 4.14/plugins/sdk" CACHE STRING "Ashita SDK Path")
# set(ASHITA_SDK_PATH "../Ashita v4 SDK 4.15/plugins/sdk" CACHE STRING "Ashita SDK Path")
set(ASHITA_SDK_PATH "../Ashita v4 SDK 4.16/plugins/sdk" CACHE STRING "Ashita SDK Path")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/versioninfo.rc.in versioninfo.rc @ONLY)

add_library(Nameplate SHARED)

target_sources(Nameplate PUBLIC
    src/Nameplate.cpp
    src/Nameplate.h

    src/Ashita/Ashita.cpp
    src/Ashita/exports.h

    src/Windozer/Windozer.cpp
    src/Windozer/Windozer.h

    src/exports.def
    "${CMAKE_CURRENT_BINARY_DIR}/versioninfo.rc"

    PUBLIC FILE_SET CXX_MODULES FILES
    src/findsignature.cppm
    )

target_include_directories(Nameplate PRIVATE ${ASHITA_SDK_PATH})
target_compile_definitions(Nameplate PRIVATE WIN32_LEAN_AND_MEAN)
set_property(TARGET Nameplate PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

if(MSVC)
    target_compile_options(Nameplate PRIVATE /EHa /W4)
else()
    target_compile_options(Nameplate PRIVATE
        -fno-rtti -march=nehalem -fomit-frame-pointer -ffast-math -mfpmath=sse -mtune=skylake
        -Wall -Wextra -Wpedantic -W
        -Wno-c++98-compat -Wno-reserved-macro-identifier -Wno-nonportable-system-include-path
        -Wno-reserved-identifier -Wno-gnu-anonymous-struct -Wno-nested-anon-types
        -Wno-c++98-compat-pedantic -Wno-non-virtual-dtor -Wno-extra-semi -Wno-shadow-field
        -Wno-old-style-cast -Wno-float-equal -Wno-documentation -Wno-microsoft-end-of-file
        -Wno-sign-conversion -Wno-cast-align
        -Wno-newline-eof -Wno-language-extension-token -Wno-cast-qual -Wno-extra-semi-stmt
        -Wno-deprecated-copy-with-user-provided-dtor -Wno-missing-noreturn
        -Wno-tautological-unsigned-zero-compare -Wno-duplicate-enum -Wno-double-promotion
        -Wno-documentation-unknown-command -Wno-unused-function
        -Wno-c99-extensions -Wno-missing-braces)
endif()
