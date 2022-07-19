cmake_minimum_required(VERSION 3.5)

project(TimeTracker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)

link_directories(${CMAKE_SOURCE_DIR}/lib)


set(PROJECT_SOURCES
        src/main.cpp
        src/login/login.cpp
        src/login/login.h
        src/login/login.ui
        src/home/home.cpp
        src/home/home.h
        src/home/home.ui
        src/store/store.cpp
        src/store/store.h
        src/activity/windowinfo.cpp
        src/activity/windowinfo.h
        src/activity/activity.h
)

if(WIN32)
    set(PROJECT_SOURCES ${PROJECT_SOURCES}
        src/activity/win.cpp
    )
elseif(APPLE)
    set(PROJECT_SOURCES ${PROJECT_SOURCES}
        src/activity/mac.mm
    )
elseif(UNIX AND NOT APPLE)
    set(PROJECT_SOURCES ${PROJECT_SOURCES}
        src/activity/linux_x11.cpp
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TimeTracker
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TimeTracker APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TimeTracker SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TimeTracker
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TimeTracker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(TimeTracker PRIVATE Qt6::Network)
target_link_directories(TimeTracker PRIVATE ${CMAKE_SOURCE_DIR}/lib)

if(APPLE)
    set(CMAKE_CXX_FLAGS "-x objective-c++")
    target_link_libraries(TimeTracker PRIVATE "-framework Cocoa")
elseif(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(TimeTracker PRIVATE X11::Xss)
    target_link_libraries(TimeTracker PRIVATE X11)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

set_target_properties(TimeTracker PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TimeTracker)
endif()