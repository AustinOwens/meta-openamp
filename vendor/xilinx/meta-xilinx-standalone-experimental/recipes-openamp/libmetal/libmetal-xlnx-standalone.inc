inherit cmake ccmake

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

OECMAKE_SOURCEPATH = "${S}/"
PROVIDES = "libmetal"

DEPENDS:append = " libxil doxygen-native xilstandalone nativesdk-xilinx-lops "
DEPENDS:arm:append = " scugic "
DEPENDS:append:xilinx-freertos = " freertos10-xilinx "
DEPENDS:remove = " sysfsutils eudev "

COMPATIBLE_HOST = ".*"

LIBMETAL_CROSS_PREFIX = "${TARGET_PREFIX}"

LIBMETAL_MACHINE:armv7r = "zynqmp_r5"
LIBMETAL_MACHINE:cortexa53 = "zynqmp_a53"
LIBMETAL_MACHINE:cortexa72 = "zynqmp_a72"
LIBMETAL_MACHINE:cortexa78 = "zynqmp_a78"
LIBMETAL_MACHINE:microblaze = "microblaze_generic"

LIBMETAL_GCC_INCLUDE:xilinx-standalone = "generic"
LIBMETAL_GCC_INCLUDE:xilinx-freertos = "freertos"

LIBMETAL_SYSTEM_NAME:xilinx-standalone = "Generic"
LIBMETAL_SYSTEM_NAME:xilinx-freertos = "FreeRTOS"

LIBMETAL_CMAKE_SYSTEM_PROCESSOR = "${TRANSLATED_TARGET_ARCH}"
LIBMETAL_CMAKE_SYSTEM_PROCESSOR:microblaze = "microblaze"

EXTRA_OECMAKE:arm = "\
    -DWITH_EXAMPLES=ON \
    -DSOC_FAMILY=${SOC_FAMILY} \
    "

TARGET_CFLAGS:armv7r:append = "  --enable-fatal-warnings=no "

LIBMETAL_ADD_DEFS = " -DWITH_DOCS=OFF -DXPAR_SCUGIC_0_DIST_BASEADDR=XPAR_SCUGIC_DIST_BASEADDR "
LIBMETAL_ADD_DEFS:microblaze = " -DWITH_DOCS=OFF "

FILES:${PN}:append = " ${libdir}/*.a "

cmake_do_generate_toolchain_file:append() {
    cat >> ${WORKDIR}/toolchain.cmake <<EOF
    set (CMAKE_SYSTEM_PROCESSOR "${LIBMETAL_CMAKE_SYSTEM_PROCESSOR}" )
    set (CROSS_PREFIX           "${LIBMETAL_CROSS_PREFIX}" CACHE STRING "")
    set (MACHINE "${LIBMETAL_MACHINE}" )
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ")
    set (CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
    set (CMAKE_SYSTEM_NAME "${LIBMETAL_SYSTEM_NAME}")
    set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER CACHE STRING "")
    set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER CACHE STRING "")
    set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER CACHE STRING "")
    include (CMakeForceCompiler)
    CMAKE_FORCE_C_COMPILER("${OECMAKE_C_COMPILER}" GNU)
    set (CMAKE_LIBRARY_PATH "${PKG_CONFIG_SYSROOT_DIR}/usr/lib" CACHE STRING "")
    set (CMAKE_C_ARCHIVE_FINISH   true)
    set (CMAKE_INCLUDE_PATH "${PKG_CONFIG_SYSROOT_DIR}/usr/include/" CACHE STRING "")
    add_definitions(${LIBMETAL_ADD_DEFS})
    include (cross-${LIBMETAL_GCC_INCLUDE}-gcc)
EOF
}
