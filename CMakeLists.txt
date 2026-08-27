cmake_minimum_required(VERSION 3.20)


project(
    aescrypt
    VERSION 1.0.0
    LANGUAGES C
)



############################################################
# Compiler Configuration
############################################################

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_EXTENSIONS OFF)



############################################################
# Include Directories
############################################################

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)



############################################################
# Main Library Sources
############################################################

set(AESCRYPT_SOURCES


    ########################################################
    # AES Core
    ########################################################

    src/aes/aes.c
    src/aes/software.c
    src/aes/ctr.c



    ########################################################
    # Backend
    ########################################################

    src/backend/detect.c
    src/backend/dispatch.c



    ########################################################
    # Container
    ########################################################

    src/container/container.c
    src/container/format.c
    src/container/writer.c
    src/container/reader.c



    ########################################################
    # Crypto
    ########################################################

    src/crypto/key.c
    src/crypto/random.c



    ########################################################
    # IO
    ########################################################

    src/io/file.c



    ########################################################
    # Platform
    ########################################################

    src/platform/platform.c
)



############################################################
# Platform Specific Sources
############################################################

if(WIN32)

    message(
        STATUS
        "Building Windows platform"
    )


    list(APPEND AESCRYPT_SOURCES

        src/platform/windows/windows_cpu.c
        src/platform/windows/random_windows.c
    )



elseif(UNIX)

    message(
        STATUS
        "Building Linux platform"
    )


    list(APPEND AESCRYPT_SOURCES

        src/platform/linux/linux_cpu.c
        src/platform/linux/random_linux.c
    )



else()

    message(
        FATAL_ERROR
        "Unsupported platform"
    )

endif()



############################################################
# AES-NI Object
#
# Compiled separately because AES intrinsics require
# processor-specific compiler flags.
############################################################

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")

    add_library(
        aescrypt_aesni
        OBJECT

        src/aes/aesni.c
    )


    if(MSVC)

        target_compile_options(
            aescrypt_aesni

            PRIVATE

            /arch:AVX
        )


    else()

        target_compile_options(
            aescrypt_aesni

            PRIVATE

            -maes
            -msse2
        )

    endif()



    list(APPEND AESCRYPT_SOURCES

        $<TARGET_OBJECTS:aescrypt_aesni>

    )

endif()



############################################################
# Static Library
############################################################

add_library(
    aescrypt
    STATIC

    ${AESCRYPT_SOURCES}
)



target_include_directories(
    aescrypt

    PUBLIC

    ${PROJECT_SOURCE_DIR}/include
)



############################################################
# Warnings
############################################################

if(MSVC)

    target_compile_options(
        aescrypt

        PRIVATE

        /W4
    )


else()

    target_compile_options(
        aescrypt

        PRIVATE

        -Wall
        -Wextra
        -Wpedantic
    )

endif()



############################################################
# Example
############################################################

add_executable(
    aescrypt_example

    examples/main.c
)



target_link_libraries(
    aescrypt_example

    PRIVATE

    aescrypt
)



############################################################
# Tests
############################################################

# Test cases are present but need code review
# after review this section would be enabled