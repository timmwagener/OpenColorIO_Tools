

cmake_minimum_required(VERSION 2.8.8)

#solution
project(ocio_view)





#CMake settings
#---------------------------------------

#Build Type 
IF (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
ENDIF (NOT CMAKE_BUILD_TYPE)

#Generator toolset (which visual studio version is used for compilation. Standard is: IDE Editing: VS2013 Compilation: VS2012)
if(MSVC OR MSVC_IDE)
	set(CMAKE_GENERATOR_TOOLSET "v110" CACHE STRING "Platform Toolset" FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON) #<-- Include current directory

#QT
#---------------------------------------
set(CUSTOM_QT_DIR "C:/symlinks/libraries/Qt/x64/msvc11/Qt/Qt5.2.1/5.2.1/msvc2012_64_opengl")
set(CMAKE_AUTOMOC ON) #<-- Auto meta object compilation for Q_Object macro
set(OPENGL_INCLUDE_PATH "C:/symlinks/microsoft_sdks/Windows/v7.0A/Lib")
set(CMAKE_PREFIX_PATH ${CUSTOM_QT_DIR}; ${OPENGL_INCLUDE_PATH})
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)

#Boost
#---------------------------------------
SET(BOOST_ROOT "C:/symlinks/boost/boost_1_54_0")
SET(BOOST_INCLUDEDIR "C:/symlinks/boost/boost_1_54_0/boost")
SET(BOOST_LIBRARYDIR "C:/symlinks/boost/boost_1_54_0/stage/x64/msvc11/python26/lib")
find_package(Boost REQUIRED)




#Headers
#---------------------------------------
file (GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	source/*.h)

#Source files
#---------------------------------------
file (GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	source/*.cpp)

#Forms (.ui files)
#---------------------------------------
file (GLOB FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	media/*.ui)








#Qt uic (compile .ui files to .h files)
#---------------------------------------
qt5_wrap_ui(FORM_HEADERS ${FORMS})


#Filters
#---------------------------------------
source_group(headers FILES ${HEADERS})
source_group(sources FILES ${SOURCES})
source_group(media FILES ${FORMS} ${FORM_HEADERS})



#project
add_executable(ocio_view ${SOURCES} ${HEADERS} ${FORM_HEADERS})

#include directories
include_directories(
	"C:/symlinks/libraries/OpenColorIO/build_harness/OpenColorIO/export"
	"C:/symlinks/libraries/OpenColorIO/build_harness/OpenColorIO_build/msvc11/x64/export"
	${Boost_INCLUDE_DIR}
	)

#target link libraries
target_link_libraries(ocio_view debug "C:/symlinks/libraries/OpenColorIO/x64/msvc11/static/debug/OpenColorIO.lib")
target_link_libraries(ocio_view optimized "C:/symlinks/libraries/OpenColorIO/x64/msvc11/static/release/OpenColorIO.lib")



#Use modules
#---------------------------------------
qt5_use_modules(ocio_view Core Widgets)