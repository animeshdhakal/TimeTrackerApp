cmake_minimum_required(VERSION 3.5)

project(TimeTracker VERSION 0.0.1 LANGUAGES CXX)

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

file(COPY images DESTINATION ${CMAKE_BINARY_DIR})

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
    src/logger/logger.cpp
    src/logger/logger.h
    src/notification/notification.h
    src/updatechecker/updatechecker.cpp
    src/updatechecker/updatechecker.h
    src/guard/RunGuard.h
    src/guard/RunGuard.cpp
    src/definations.h
)

if(WIN32)
    set(PROJECT_SOURCES ${PROJECT_SOURCES}
        src/activity/win.cpp
        src/notification/win_notification.cpp
        src/notification/wintoastlib.h
        src/notification/wintoastlib.cpp
        TimeTracker.rc
    )
elseif(APPLE)
    set(PROJECT_SOURCES ${PROJECT_SOURCES}
        src/activity/mac.mm
        src/notification/mac_notification.cpp
    )
elseif(UNIX AND NOT APPLE)
    set(PROJECT_SOURCES ${PROJECT_SOURCES}
        src/activity/linux_x11.cpp
        src/notification/linux_notification.cpp
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
target_compile_definitions(TimeTracker PUBLIC VERSION="${PROJECT_VERSION}")

if(WIN32)
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "-x objective-c++")
    target_link_libraries(TimeTracker PRIVATE "-framework Cocoa")
elseif(UNIX AND NOT APPLE)
    include(FindPkgConfig)
    pkg_check_modules(LIBNOTIFY REQUIRED libnotify)
    pkg_check_modules(GLIB REQUIRED glib-2.0)

    find_package(X11 REQUIRED)

    target_link_libraries(TimeTracker PRIVATE X11::Xss)
    target_link_libraries(TimeTracker PRIVATE X11)
    target_link_libraries(TimeTracker PRIVATE ${GLIB_LIBRARIES})
    target_link_libraries(TimeTracker PRIVATE ${LIBNOTIFY_LIBRARIES})
    target_link_libraries(TimeTracker PRIVATE ${GLIB_LIBRARIES})

    target_include_directories(TimeTracker PRIVATE ${GLIB_INCLUDE_DIRS})
    target_include_directories(TimeTracker PRIVATE ${LIBNOTIFY_INCLUDE_DIRS})
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
