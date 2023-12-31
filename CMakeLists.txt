cmake_minimum_required(VERSION 3.5)

project(MyRaytracer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        Mainwindow.cpp
        Mainwindow.h
        Mainwindow.ui
        Ray.h
        Hittable.h
        Sphere.h
        Common.h
        Interval.h
        Interval.cpp
        Camera.h
        Camera.cpp
        Material.h
        Worker.h
        Worker.cpp
        ImageComputationData.h
        ImageComputationData.cpp
        World.h
        World.cpp
        HitRecord.h
        HitRecord.cpp
        QVector3DUtils.cpp
        QVector3DUtils.h
        CommonUtils.cpp
        CommonUtils.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MyRaytracer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MyRaytracer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MyRaytracer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MyRaytracer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(MyRaytracer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(MyRaytracer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MyRaytracer)
endif()
