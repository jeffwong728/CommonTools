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

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets LinguistTools REQUIRED)

list(APPEND QT_RESOURCES resources/resources.qrc)
qt_add_resources(RESOURCES ${QT_RESOURCES})
list(APPEND RESOURCES resources/resources.rc)

set(TS_FILES TestQT_zh_CN.ts)

if(ANDROID)
  add_library(TestQT SHARED
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    ${TS_FILES}
  )
else()
  add_executable(TestQT ${GUI_TYPE}
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    ${TS_FILES}
    ${RESOURCES}
  )
endif()

target_link_libraries(TestQT PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
