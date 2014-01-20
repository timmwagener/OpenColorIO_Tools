

cmake_minimum_required(VERSION 2.8.8)

#solution
#---------------------------------------
project(ocio_maya)





#CMake settings
#---------------------------------------

#Build Type 
IF (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
ENDIF (NOT CMAKE_BUILD_TYPE)

#Generator toolset (which visual studio version is used for compilation. Standard is: IDE Editing: VS2013 Compilation: VS2010)
if(MSVC OR MSVC_IDE)
	set(CMAKE_GENERATOR_TOOLSET "v100" CACHE STRING "Platform Toolset" FORCE)
endif()

#CMAKE Variables
#maya
set(MAYA_DIR "C:/symlinks/maya/maya2012x64" CACHE PATH "Path to Maya install. For example: C:/Program Files/Autodesk/Maya2012")
#ocio
set(OCIO_INCLUDE_DIR "C:/symlinks/libraries/OpenColorIO/build_harness/OpenColorIO/export" CACHE PATH "OCIO default include path. Must contain the files: OpenColorIO.h, OpenColorTransforms.h and OpenColorTypes.h")
set(OCIO_ABI_INCLUDE_DIR "C:/symlinks/libraries/OpenColorIO/build_harness/OpenColorIO_build/msvc10/x64/export" CACHE PATH "OCIO ABI header include path. This header is built by cmake and therefore can be found in the build directory. Must contain the file: OpenColorABI.h")
set(OCIO_LIBRARY_DIR_DEBUG "C:/symlinks/libraries/OpenColorIO/x64/msvc10/static/debug/OpenColorIO.lib" CACHE FILEPATH "OCIO library to link against")
set(OCIO_LIBRARY_DIR_RELEASE "C:/symlinks/libraries/OpenColorIO/x64/msvc10/static/release/OpenColorIO.lib" CACHE FILEPATH "OCIO library to link against")
#boost
set(BOOST_ROOT "C:/symlinks/boost/boost_1_54_0" CACHE PATH "Boost Root, for example: C:/boost_1_54_0")
set(BOOST_INCLUDEDIR "C:/symlinks/boost/boost_1_54_0/boost" CACHE PATH "Boost include dir, for example: C:/boost_1_54_0/boost")
set(BOOST_LIBRARYDIR "C:/symlinks/boost/boost_1_54_0/stage/x64/msvc10/python26/lib" CACHE PATH "Boost library dir, for example: C:/boost_1_54_0/stage/x64/msvc10/python27/lib")
#custom
set(ADDITIONAL_SOURCE_DIR_1 "C:/symlinks/paper_sketch/OpenColorIO_tools/ocio_functionality/source" CACHE PATH "Additional include directories. Will be scanned for .h and .cpp files")
set(ADDITIONAL_SOURCE_DIR_2 "" CACHE PATH "Additional include directories. Will be scanned for .h and .cpp files")
set(OCIO_FUNCTIONALITY_INCLUDE_DIR "C:/symlinks/paper_sketch/OpenColorIO_tools/ocio_functionality/source" CACHE FILEPATH "Additional include directory")


#Find packages
#-------------------------------------------------------------
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED)





#Headers
#---------------------------------------
file(GLOB HEADERS 
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h 
	${ADDITIONAL_SOURCE_DIR_1}/*.h
	${ADDITIONAL_SOURCE_DIR_2}/*.h
	)


#Source files
#---------------------------------------
file (GLOB SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${ADDITIONAL_SOURCE_DIR_1}/*.cpp
	${ADDITIONAL_SOURCE_DIR_2}/*.cpp
	)




#Filters
#---------------------------------------
source_group(headers FILES ${HEADERS})
source_group(sources FILES ${SOURCES})





#Add library
#---------------------------------------

#project
add_library(ocio_maya SHARED ${SOURCES} ${HEADERS})
#set suffix to .mll
SET_TARGET_PROPERTIES(ocio_maya PROPERTIES SUFFIX .mll)
#preprocessor definitions
add_definitions(-DWIN32 -DNDEBUG -D_WINDOWS -DNT_PLUGIN -DREQUIRE_IOSTREAM)

#include directories
include_directories(
	${OCIO_INCLUDE_DIR}
	${OCIO_ABI_INCLUDE_DIR}
	"${MAYA_DIR}/include"
	${OCIO_FUNCTIONALITY_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
	)

#target link libraries debug
target_link_libraries(ocio_maya debug
	#Maya
	${OCIO_LIBRARY_DIR_DEBUG}
	"${MAYA_DIR}/lib/Foundation.lib"
	"${MAYA_DIR}/lib/OpenMaya.lib"
	"${MAYA_DIR}/lib/OpenMayaUI.lib"
	"${MAYA_DIR}/lib/OpenMayaAnim.lib"
	"${MAYA_DIR}/lib/OpenMayaFX.lib"
	"${MAYA_DIR}/lib/OpenMayaRender.lib"
	"${MAYA_DIR}/lib/Image.lib"
	#OpenGL
	opengl32.lib
	glu32.lib
	)

#target link libraries release
target_link_libraries(ocio_maya optimized
	#Maya
	${OCIO_LIBRARY_DIR_RELEASE}
	"${MAYA_DIR}/lib/Foundation.lib"
	"${MAYA_DIR}/lib/OpenMaya.lib"
	"${MAYA_DIR}/lib/OpenMayaUI.lib"
	"${MAYA_DIR}/lib/OpenMayaAnim.lib"
	"${MAYA_DIR}/lib/OpenMayaFX.lib"
	"${MAYA_DIR}/lib/OpenMayaRender.lib"
	"${MAYA_DIR}/lib/Image.lib"
	#OpenGL
	opengl32.lib
	glu32.lib
	)