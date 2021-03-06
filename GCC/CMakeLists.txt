PROJECT(GCC C)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

IF(Path64_INSTALL_COMPONENT_NAME_RUNTIME_EXECUTABLE)
  SET(Path64_INSTALL_COMPONENT_RUNTIME_EXECUTABLE
    COMPONENT ${Path64_INSTALL_COMPONENT_NAME_RUNTIME_EXECUTABLE})
ENDIF(Path64_INSTALL_COMPONENT_NAME_RUNTIME_EXECUTABLE)

# Do not place internal build-related executables in a parent project
# executable path.  Backup the real executable output path so it can
# be restored in gcc/cp for building cc1plus42.
SET(GCC_EXECUTABLE_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${GCC_BINARY_DIR}/gcc")

# Do not inherit library building rules from the parent project.
SET(BUILD_SHARED_LIBS 0)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${GCC_BINARY_DIR}/gcc")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${GCC_BINARY_DIR}/gcc")

IF(APPLE)
  IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-cpp-precomp")
  ENDIF(CMAKE_COMPILER_IS_GNUCC)
ENDIF(APPLE)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_ANSI_CFLAGS}")

IF(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  STRING(REGEX REPLACE "/W([0-4])?" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  SET(CMAKE_CXX_WARNING_LEVEL 0)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W0")
ELSE()
  IF(CMAKE_C_COMPILER MATCHES "bcc32")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w-")
  ELSE(CMAKE_C_COMPILER MATCHES "bcc32")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
  ENDIF(CMAKE_C_COMPILER MATCHES "bcc32")
ENDIF()
# Unknown what happens if you remove TARG_X8664
ADD_DEFINITIONS(-DALMOST_STDC
                -DIs_True_On
                -DHAVE_CONFIG_H
#		-DTARG_X8664
		-DFE_GNU_4_2_0
		-DKEY
                -W
                -Wall
                -Wwrite-strings
                -Wstrict-prototypes
                -Wmissing-prototypes
                -Wmissing-format-attribute)

SUBDIRS(config_cmake libiberty libdecnumber libcpp libspin gcc)
