cmake_minimum_required(VERSION 3.9)
project(mpx_spring2018_above_c_level)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/core/asm.h
        include/core/interrupts.h
        include/core/io.h
        include/core/serial.h
        include/core/tables.h
        include/mem/heap.h
        include/mem/paging.h
        include/string.h
        include/system.h
        kernel/core/interrupts.c
        kernel/core/kmain.c
        kernel/core/serial.c
        kernel/core/system.c
        kernel/core/tables.c
        kernel/mem/heap.c
        kernel/mem/paging.c
        lib/string.c
        modules/mpx_supt.c
        modules/mpx_supt.h
        modules/polling.c
        modules/polling.h
        modules/comhand.c
        modules/comhand.h modules/processcommand.c modules/processcommand.h)

include_directories(include .)

add_executable(mpx_spring2018_above_c_level ${SOURCE_FILES})

