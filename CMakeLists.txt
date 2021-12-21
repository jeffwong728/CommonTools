cmake_minimum_required(VERSION 3.5)

project(TestQT LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_USE_QTMAIN TRUE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(GUI_TYPE WIN32)
    add_compile_options(/MP)
    add_compile_definitions($<$<CONFIG:Debug>:_DEBUG>)
    add_definitions(-DUNICODE -D_UNICODE -U_MBCS)
    add_compile_options("/utf-8")
elseif(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++17)
endif(MSVC)

set(APPLICATION_NAME ${TARGET_NAME})
set(APPLICATION_VERSION ${VERSION})
set(APPLICATION_ORGANIZATION "Menari Softworks")
set(APPLICATION_ORGANIZATION_DOMAIN "https://github.com/nholthaus/")

configure_file(src/appinfo.h.in appinfo.h @ONLY NEWLINE_STYLE UNIX)

source_group(generated REGULAR_EXPRESSION ".*_automoc.cpp")
source_group(resources REGULAR_EXPRESSION ".*[.][q]?rc")
source_group(source REGULAR_EXPRESSION ".*[.]cpp")
source_group(headers REGULAR_EXPRESSION ".*[.]h")
source_group(continuous-integration REGULAR_EXPRESSION ".*[.]yml")

set(PYBIND11_NOPYTHON ON)
find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
find_package(pybind11 CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets LinguistTools Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets LinguistTools Network REQUIRED)

#set(PYBIND11_NOPYTHON ON)
#include_directories(${CMAKE_CURRENT_LIST_DIR})
#add_subdirectory(pybind11)

message(STATUS "Found the following Python 3 libraries:")
message(STATUS "    Python 3 Version: ${Python3_VERSION}")
message(STATUS "    Python 3 include directories: ${Python3_INCLUDE_DIRS}")
message(STATUS "    Python 3 libraries: ${Python3_LIBRARIES}")
message(STATUS "    Python 3 debug library: ${Python3_LIBRARY_DEBUG}")
message(STATUS "    Python 3 release library: ${Python3_LIBRARY_RELEASE}")
message(STATUS "    Python 3 compile options: ${Python3_COMPILE_OPTIONS}")
message(STATUS "    Python 3 link options: ${Python3_LINK_OPTIONS}")
message(STATUS "    Python 3 library directories: ${Python3_LIBRARY_DIRS}")
message(STATUS "    Python 3 runtime library directories: ${Python3_RUNTIME_LIBRARY_DIRS}")

get_target_property(Python3_LOCATION Python3::Python LOCATION)
get_target_property(Python3_LOCATION_DEBUG Python3::Python LOCATION_DEBUG)
get_target_property(Python3_LOCATION_RELEASE Python3::Python LOCATION_RELEASE)
message(STATUS "    Python 3 LOCATION: ${Python3_LOCATION}")
message(STATUS "    Python 3 LOCATION_DEBUG: ${Python3_LOCATION_DEBUG}")
message(STATUS "    Python 3 LOCATION_RELEASE: ${Python3_LOCATION_RELEASE}")

list(APPEND QT_RESOURCES resources/resources.qrc)
qt_add_resources(RESOURCES ${QT_RESOURCES})
list(APPEND RESOURCES resources/resources.rc)

set(TS_FILES TestQT_zh_CN.ts)


add_executable(TestQT ${GUI_TYPE}
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    ${TS_FILES}
    ${RESOURCES}
)

add_executable(Stub ${GUI_TYPE}
    src/stub.cpp
    src/dialog.h
    src/dialog.cpp
)

add_library(camera_plugin SHARED src/icamera_global.h src/icamera.h)
target_link_libraries(camera_plugin PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui opencv_core)
target_compile_definitions(camera_plugin PRIVATE -DICAMERA_LIBRARY -DQT_DEPRECATED_WARNINGS)

add_library(videofile_plugin SHARED src/videofile_global.h src/videofile.h src/videofile.cpp src/videofilemanager.h src/videofilemanager.cpp)
target_link_libraries(videofile_plugin PRIVATE camera_plugin Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui opencv_core opencv_videoio)
target_compile_definitions(videofile_plugin PRIVATE -DVIDEOFILE_LIBRARY -DQT_DEPRECATED_WARNINGS)

target_link_libraries(TestQT PUBLIC ${Python3_LIBRARY_RELEASE})
target_include_directories(TestQT PRIVATE ${Python3_INCLUDE_DIRS})
#target_link_directories(TestQT PRIVATE ${Python3_LIBRARY_DIRS})
target_link_options(TestQT PRIVATE ${Python3_LINK_OPTIONS})
target_link_libraries(TestQT PRIVATE pybind11::embed)
target_link_libraries(TestQT PRIVATE camera_plugin opencv_core)
target_link_libraries(TestQT PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_compile_definitions(TestQT PRIVATE -DQT_DEPRECATED_WARNINGS)

target_link_libraries(Stub PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_compile_definitions(Stub PRIVATE -DQT_DEPRECATED_WARNINGS)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
