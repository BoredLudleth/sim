cmake_minimum_required(VERSION 3.10)
project(RiscVSimulator VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_subdirectory(thirdparty/ELFIO)

add_executable(riscv-simulator
    src/main.cpp
    src/loader.cpp
    src/machine.cpp
    src/hart.cpp
    src/memory.cpp
    src/generated_instructions.cpp
    src/cached.cpp
    src/mmu.cpp
)

option(ENABLE_CACHE "Enable cache in simulator" OFF)
option(ENABLE_MMU "Enable mmu in simulator" OFF)

if(ENABLE_CACHE)
    target_compile_definitions(riscv-simulator PRIVATE ENABLE_CACHE)
    message(STATUS "Cache enabled")
else()
    message(STATUS "Cache disabled")
endif()

if(ENABLE_MMU)
    target_compile_definitions(riscv-simulator PRIVATE ENABLE_MMU)
    message(STATUS "MMU enabled")
else()
    message(STATUS "MMU disabled")
endif()

target_include_directories(riscv-simulator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(riscv-simulator PRIVATE
    src
    thirdparty/ELFIO
)

target_link_libraries(riscv-simulator PRIVATE elfio)

include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT error_message)

if(IPO_SUPPORTED)
    message(STATUS "LTO (IPO) supported")

    set_property(TARGET riscv-simulator 
        PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    set_property(TARGET riscv-simulator 
        PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE)

    target_compile_options(riscv-simulator PRIVATE
        $<$<AND:$<CONFIG:Release>,$<BOOL:${IPO_SUPPORTED}>>:-flto>
        $<$<AND:$<CONFIG:RelWithDebInfo>,$<BOOL:${IPO_SUPPORTED}>>:-flto>
    )
else()
    message(STATUS "LTO (IPO) not supported: ${error_message}")
endif()

target_compile_options(riscv-simulator PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:RelWithDebInfo>:-O2 -g>
    $<$<CONFIG:Debug>:-O0 -g3>
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(riscv-simulator PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<AND:$<CONFIG:Release>,$<BOOL:${IPO_SUPPORTED}>>:-flto=auto -fuse-linker-plugin>
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(riscv-simulator PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<AND:$<CONFIG:Release>,$<BOOL:${IPO_SUPPORTED}>>:-flto=thin>
    )
endif()
