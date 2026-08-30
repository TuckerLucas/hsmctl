cmake_minimum_required(VERSION 3.21.0)
project(hsmctl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

# ==== SQLite3 ====
find_package(SQLite3 REQUIRED)

# ==== OpenSSL ====
find_package(OpenSSL 3.0 REQUIRED)

# ==== Catch2 ====
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)

# ==== libtropic ====
FetchContent_Declare(
    libtropic
    GIT_REPOSITORY https://github.com/tropicsquare/libtropic.git
    GIT_TAG v4.0.0
)

# ==== mbedTLS (required by libtropic) ====
set(ENABLE_TESTING OFF CACHE BOOL "")
set(ENABLE_PROGRAMS OFF CACHE BOOL "")
FetchContent_Declare(
    mbedtls_v4
    URL https://github.com/Mbed-TLS/mbedtls/releases/download/mbedtls-4.0.0/mbedtls-4.0.0.tar.bz2
    URL_HASH SHA256=2f3a47f7b3a541ddef450e4867eeecb7ce2ef7776093f3a11d6d43ead6bf2827
)

FetchContent_MakeAvailable(Catch2 libtropic mbedtls_v4)

# ==== Wire mbedTLS into libtropic ====
target_link_libraries(tropic PUBLIC mbedtls)

# ==== Add CAL and HAL ====
add_subdirectory(${libtropic_SOURCE_DIR}/cal/mbedtls_v4 "mbedtls_v4_cal")
target_sources(tropic PRIVATE ${LT_CAL_SRCS})
target_include_directories(tropic PUBLIC ${LT_CAL_INC_DIRS})

add_subdirectory(${libtropic_SOURCE_DIR}/hal/linux/spi "linux_spi_hal")
target_sources(tropic PRIVATE ${LT_HAL_SRCS})
target_include_directories(tropic PUBLIC ${LT_HAL_INC_DIRS})

# ==== SPI/GPIO device paths ====
if(NOT DEFINED LT_SPI_DEV_PATH)
    set(LT_SPI_DEV_PATH "/dev/spidev0.0")
endif()
if(NOT DEFINED LT_GPIO_DEV_PATH)
    set(LT_GPIO_DEV_PATH "/dev/gpiochip0")
endif()

# ==== SH0 pairing keys ====
set(LT_SH0_KEYS "prod0" CACHE STRING "")
set(LT_USE_SH0_ENG_SAMPLE 0 CACHE INTERNAL "")
set(LT_USE_SH0_PROD0 0 CACHE INTERNAL "")
if(LT_SH0_KEYS STREQUAL "eng_sample")
    set(LT_USE_SH0_ENG_SAMPLE 1)
elseif(LT_SH0_KEYS STREQUAL "prod0")
    set(LT_USE_SH0_PROD0 1)
endif()

# ==== CTest ====
include(CTest)
enable_testing()

# ==== Subdirectories ====
add_subdirectory(src)