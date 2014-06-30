# CMakeLists.txt
#
# $Id: CMakeLists.txt 31995 2010-02-24 22:32:10Z jmayer $
#
# Wireshark - Network traffic analyzer
# By Gerald Combs <gerald@wireshark.org>
# Copyright 1998 Gerald Combs
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
IF(DEFINED CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of
build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug
Release RelWithDebInfo MinSizeRel.")
ELSE()
   SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build,
options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release
RelWithDebInfo MinSizeRel.")
ENDIF()


project(broker C)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_BACKWARDS_COMPATIBILITY 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_C_FLAGS_DEBUG "-g -ggdb")

IF(EXISTS /usr/lib/wireshark/libwireshark1/plugins/)
set(CMAKE_INSTALL_LIBDIR /usr/lib/wireshark/libwireshark1/plugins/)
ELSE()
set(CMAKE_INSTALL_LIBDIR ~/.wireshark)
ENDIF()

INCLUDE(UseMakeDissectorReg)
include(CMakeForceCompiler)
  
set(GLIB2_MIN_VERSION 2.4.0)

IF(NOT DEFINED ENV{GLIB2_INCLUDE_DIRS})
find_package(GLIB2)
ELSE()
SET(GLIB2_INCLUDE_DIRS $ENV{GLIB2_INCLUDE_DIRS})
ENDIF()
include_directories (${GLIB2_INCLUDE_DIRS})

IF(NOT DEFINED WIRESHARK_INCLUDE_DIRS)
find_package(Wireshark)
ELSE()
SET(GLIB2_INCLUDE_DIRS $ENV{WIRESHARK_INCLUDE_DIRS})
ENDIF()
include_directories (${WIRESHARK_INCLUDE_DIRS})


include_directories (/usr/include/apr-1.0)



set(LINK_MODE_LIB SHARED)
set(LINK_MODE_MODULE MODULE)


set(DISSECTOR_SRC
	src/broker.c
)

set(PLUGIN_FILES
	plugin.c
	${DISSECTOR_SRC}
)

set(CLEAN_FILES
	${PLUGIN_FILES}
)

if (WERROR)
	set_source_files_properties(
		${CLEAN_FILES}
		PROPERTIES
		COMPILE_FLAGS -Werror
	)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

register_dissector_files(plugin.c
	plugin
	${DISSECTOR_SRC}
)

add_library(broker ${LINK_MODE_MODULE}
	${PLUGIN_FILES}
)
set_target_properties(broker PROPERTIES PREFIX "")
set_target_properties(broker PROPERTIES LINK_FLAGS "${WS_LINK_FLAGS}")

IF(DEFINED MANUAL_LINK)
target_link_libraries(broker jansson apr-1)
ELSE()
target_link_libraries(broker jansson apr-1)
ENDIF()


install(TARGETS broker
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/plugins NAMELINK_SKIP
)

#IF(DEFINED GNU_HOST)
# CMAKE_FORCE_C_COMPILER(  ${GNU_HOST}-gcc GNU)
# set(CMAKE_STRIP "${GNU_HOST}-strip")
#ENDIF()

IF(DEFINED STATIC_GCC_BUILD)
 set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -static-libgcc")
ENDIF()

#add_custom_command(TARGET broker
# POST_BUILD
# COMMAND ${CMAKE_STRIP} --strip-debug --strip-unneeded broker.so
#)


