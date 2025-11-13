#
# Copyright (c) 2025 LMT
#

cmake_minimum_required(VERSION 3.20.0)

# Include lmtSDK
set(EXTRA_ZEPHYR_MODULES "${CMAKE_CURRENT_SOURCE_DIR}/../..")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(lmtSDK_eval_kit_sample)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
