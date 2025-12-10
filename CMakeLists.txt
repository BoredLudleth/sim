cmake_minimum_required(VERSION 3.10)
project(RiscVSimulator VERSION 1.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT error_message)

if(IPO_SUPPORTED)
    message(STATUS "LTO (IPO) supported")
    
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
    
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto=auto")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto=auto")
else()
    message(WARNING "LTO (IPO) doesn't support")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

add_subdirectory(thirdparty/ELFIO)

add_executable(riscv-simulator
    src/main.cpp
    src/loader.cpp
    src/machine.cpp
    src/hart.cpp
    src/memory.cpp
    src/generated_instructions.cpp
    src/cached.cpp
)

if(IPO_SUPPORTED)
    set_property(TARGET riscv-simulator PROPERTY INTERPROCEDURAL_OPTIMIZATION ON)
endif()

target_include_directories(riscv-simulator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(riscv-simulator PRIVATE elfio)

target_include_directories(riscv-simulator PRIVATE
    src
    thirdparty/ELFIO
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(riscv-simulator PRIVATE
        $<$<CONFIG:Release>:-flto=auto -fuse-linker-plugin>
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(riscv-simulator PRIVATE
        $<$<CONFIG:Release>:-flto=thin>
    )
endif()