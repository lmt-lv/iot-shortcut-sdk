#
# Copyright (c) 2025 LMT
#
list(APPEND CMAKE_MODULE_PATH ${ZEPHYR_BASE}/modules/nanopb)
include(nanopb)

#zephyr_nanopb_sources(app boards/arm/lmt_som_nrf9160/lib/extra/A2.proto)

message(STATUS "Called from parent CMakeLists.txt at: ${CMAKE_SOURCE_DIR}")
if(CONFIG_ARDUINO_STYLE)
    message(STATUS "Arduino style programming is enabled.")
    if(EXISTS "${CMAKE_SOURCE_DIR}/src/main.c")
        message(FATAL_ERROR "Error: The file '${CMAKE_SOURCE_DIR}/src/main.c' must not exist for Arduino style project.")
    endif()
else()
    message(STATUS "Arduino style programming is NOT enabled.")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/src/main.c")
        message(FATAL_ERROR "Error: The file '${CMAKE_SOURCE_DIR}/src/main.c' must exist for C style project.")
    endif()
endif()

add_library(lmtSDK STATIC IMPORTED GLOBAL)

if (CONFIG_THREAD_ANALYZER)
    if (CONFIG_FP_HARDABI)
        if (CONFIG_CPU_CORTEX_M33)
            #fpv5-sp-d16-hard
            set_target_properties(lmtSDK PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/${GCC_M_CPU}/liblmtSDK.a)
        elseif(CONFIG_CPU_CORTEX_M4)
            #fpv4-sp-d16-hard
            set_target_properties(lmtSDK PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/${GCC_M_CPU}/liblmtSDK.a)
        else()
            assert(0 "Unsupported configuration.")
        endif()
    else()
        #zephyr_library_compile_definitions(BME68X_DO_NOT_USE_FPU)
        if (CONFIG_CPU_CORTEX_M33 OR CONFIG_CPU_CORTEX_M4)
            set_target_properties(lmtSDK PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/liblmtSDK.a)
        else()
            assert(0 "Unsupported configuration.")
        endif()
    endif()
else()
    if (CONFIG_FP_HARDABI)
        if (CONFIG_CPU_CORTEX_M33)
            #fpv5-sp-d16-hard
            set_target_properties(lmtSDK PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${GCC_M_CPU}/liblmtSDK.a)
        elseif(CONFIG_CPU_CORTEX_M4)
            #fpv4-sp-d16-hard
            set_target_properties(lmtSDK PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${GCC_M_CPU}/liblmtSDK.a)
        else()
            assert(0 "Unsupported configuration.")
        endif()
    else()
        #zephyr_library_compile_definitions(BME68X_DO_NOT_USE_FPU)
        if (CONFIG_CPU_CORTEX_M33 OR CONFIG_CPU_CORTEX_M4)
            set_target_properties(lmtSDK PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/liblmtSDK.a)
        else()
            assert(0 "Unsupported configuration.")
        endif()
    endif()
endif()

#<2.7.0
target_link_libraries(lmtSDK INTERFACE zephyr_interface)
#>=2.7.0
#target_link_libraries(lmtSDK INTERFACE Zephyr::Zephyr)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(app PRIVATE lmtSDK)

# Prepare for DDT FS
set(PM_CONFIG_H "${CMAKE_BINARY_DIR}/zephyr/include/generated/pm_config.h")

set(GEN_LMT_DDT_FS_START_PY "${CMAKE_CURRENT_SOURCE_DIR}/scripts/gen_lmt_ddt_fs_start.py")
set(GEN_LMT_DDT_FS_NVS_PY "${CMAKE_CURRENT_SOURCE_DIR}/scripts/gen_lmt_ddt_fs_nvs.py")
set(GEN_LMT_DDT_FS_LFS_PY "${CMAKE_CURRENT_SOURCE_DIR}/scripts/gen_lmt_ddt_fs_lfs.py")

set(LMT_GEN_C_DIR "${CMAKE_CURRENT_BINARY_DIR}/lmt_generated")
set(LMT_DDT_FS_C "${LMT_GEN_C_DIR}/lmt_ddt_fs.c")

file(MAKE_DIRECTORY "${LMT_GEN_C_DIR}")

# Generate the C file start
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lmt_ddt_fs.c.start.ok
    COMMAND ${PYTHON_EXECUTABLE} ${GEN_LMT_DDT_FS_START_PY}
            ${LMT_GEN_C_DIR}
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/lmt_ddt_fs.c.start.ok
    DEPENDS ${PM_CONFIG_H} ${GEN_LMT_DDT_FS_START_PY}
    COMMENT "Generating start of lmt_ddt_fs.c"
    VERBATIM
)

add_custom_target(generate_lmt_ddt_fs_start DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lmt_ddt_fs.c.start.ok)

# Generate the NVS section
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lmt_ddt_fs.c.nvs.ok
    COMMAND ${PYTHON_EXECUTABLE} ${GEN_LMT_DDT_FS_NVS_PY}
            ${LMT_GEN_C_DIR} ${PM_CONFIG_H} nvs_storage fs_nvs
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/lmt_ddt_fs.c.nvs.ok
    DEPENDS ${PM_CONFIG_H} ${GEN_LMT_DDT_FS_NVS_PY}
    COMMENT "Appending NVS section"
    VERBATIM
)

add_custom_target(generate_lmt_ddt_fs_nvs DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lmt_ddt_fs.c.nvs.ok)
add_dependencies(generate_lmt_ddt_fs_nvs generate_lmt_ddt_fs_start)

# Generate the LFS section
dt_node_exists(LFS1_PRESENT PATH /fstab/lfs1)
if(LFS1_PRESENT)

    add_custom_command(
        OUTPUT ${LMT_DDT_FS_C}
        COMMAND ${PYTHON_EXECUTABLE} ${GEN_LMT_DDT_FS_LFS_PY}
                ${LMT_GEN_C_DIR} ${PM_CONFIG_H}
                littlefs_storage fs_lfs_config fs_lfs_mount /lfs
        DEPENDS ${PM_CONFIG_H} ${GEN_LMT_DDT_FS_LFS_PY}
        COMMENT "Appending LFS section"
    )

    add_custom_target(generate_lmt_ddt_fs_lfs DEPENDS ${LMT_DDT_FS_C})
    add_dependencies(generate_lmt_ddt_fs_lfs generate_lmt_ddt_fs_nvs)

    add_custom_target(generate_lmt_ddt_fs ALL DEPENDS generate_lmt_ddt_fs_lfs)

else()
    message(STATUS "Log filesystem in internal storage")
    #ToDo: more info needed
    # storage_partition
    add_custom_target(generate_lmt_ddt_fs ALL DEPENDS generate_lmt_ddt_fs_nvs)
endif()

# 1. Generate json.bin from /etc
set(ETC_FOLDER "${CMAKE_SOURCE_DIR}/etc")
set(GEN_JSON_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/gen_json_bin.py")
set(JSON_BIN "${CMAKE_CURRENT_BINARY_DIR}/json.bin")

file(GLOB ETC_FILES "${ETC_FOLDER}/*.json")

add_custom_command(
    OUTPUT ${JSON_BIN}
    COMMAND ${PYTHON_EXECUTABLE} ${GEN_JSON_SCRIPT}
            --input-dir ${ETC_FOLDER}
            --output-file ${JSON_BIN}
            --max-size 8192
    DEPENDS ${GEN_JSON_SCRIPT} ${ETC_FILES}
    COMMENT "Generating JSON binary file (json.bin) from ${ETC_FOLDER}"
    VERBATIM
)

add_custom_target(generate_json_bin ALL DEPENDS ${JSON_BIN})
add_dependencies(generate_json_bin generate_lmt_ddt_fs)

# 2. Convert json.bin into a C source file containing uint8_t coap[] = { ... };
set(VAR_NAME coap_data)
set(BIN2C_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/bin2c_array.py")
set(C_BIN_ARRAY_C "${LMT_GEN_C_DIR}/${VAR_NAME}_array.c")
set(C_BIN_ARRAY_H "${LMT_GEN_C_DIR}/${VAR_NAME}_array.h")

add_custom_command(
    OUTPUT ${C_BIN_ARRAY_C} ${C_BIN_ARRAY_H}
    COMMAND ${PYTHON_EXECUTABLE} ${BIN2C_SCRIPT} ${JSON_BIN} ${VAR_NAME} -o ${LMT_GEN_C_DIR}
    DEPENDS ${JSON_BIN} ${BIN2C_SCRIPT}
    COMMENT "Generating C array source and header from ${JSON_BIN}"
    VERBATIM
)

add_custom_target(generate_c_array ALL DEPENDS ${C_BIN_ARRAY_C} ${C_BIN_ARRAY_H})
add_dependencies(generate_c_array generate_json_bin)

# 3. Include generated .c file in your build
set(LMTSDK_GEN_SOURCES
    ${LMT_DDT_FS_C}
    ${C_BIN_ARRAY_C}
)
target_sources(app PRIVATE ${LMTSDK_GEN_SOURCES})
target_include_directories(app PRIVATE ${LMT_GEN_C_DIR})
add_dependencies(app generate_c_array)
