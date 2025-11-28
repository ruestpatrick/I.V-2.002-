cmake_minimum_required(VERSION 3.20)

project(ToyotaMultimediaSystem
    VERSION 2026.1.0
    DESCRIPTION "Toyota Corolla 2026 Multimedia Infotainment System"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(ENABLE_TESTING "Enable unit tests" ON)
option(ENABLE_ASAN "Enable Address Sanitizer" OFF)
option(ENABLE_TSAN "Enable Thread Sanitizer" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(BUILD_HMI "Build HMI application" ON)
option(BUILD_SERVICES "Build backend services" ON)

# Automotive-specific options
option(ENABLE_CAN "Enable CAN bus support" ON)
option(ENABLE_OTA "Enable OTA updates" ON)
option(ENABLE_CARPLAY "Enable Apple CarPlay" ON)
option(ENABLE_ANDROID_AUTO "Enable Android Auto" ON)
option(ENABLE_DIAGNOSTICS "Enable diagnostic interface" ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# MISRA C++ compliance flags
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++ -Wshadow -Wcast-qual")
endif()

# Address Sanitizer
if(ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

# Thread Sanitizer
if(ENABLE_TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
endif()

# Code coverage
if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# Find required packages
find_package(Qt6 6.5 REQUIRED COMPONENTS 
    Core 
    Gui 
    Quick 
    Qml 
    Multimedia 
    DBus
    Network
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(PULSEAUDIO REQUIRED libpulse)

if(ENABLE_CAN)
    pkg_check_modules(SOCKETCAN REQUIRED libsocketcan)
endif()

# OpenSSL for cryptography
find_package(OpenSSL REQUIRED)

# JSON library
find_package(nlohmann_json 3.10 REQUIRED)

# Boost libraries
find_package(Boost 1.75 REQUIRED COMPONENTS 
    system 
    filesystem 
    thread
    chrono
)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${Qt6_INCLUDE_DIRS}
    ${GSTREAMER_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

# Core library
add_library(toyota_multimedia_core SHARED
    src/core/system_controller.cpp
    src/core/configuration_manager.cpp
    src/core/telemetry_manager.cpp
    src/core/security_manager.cpp
)

target_link_libraries(toyota_multimedia_core
    Qt6::Core
    Qt6::DBus
    OpenSSL::SSL
    OpenSSL::Crypto
    Boost::system
    Boost::filesystem
    nlohmann_json::nlohmann_json
)

# OTA update service
if(ENABLE_OTA)
    add_library(toyota_ota SHARED
        src/ota/ota_manager.cpp
        src/ota/secure_boot.cpp
        src/ota/partition_manager.cpp
        src/ota/update_package.cpp
    )
    
    target_link_libraries(toyota_ota
        toyota_multimedia_core
        Qt6::Network
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()

# Connectivity services
add_library(toyota_connectivity SHARED
    src/connectivity/bluetooth_manager.cpp
    src/connectivity/wifi_manager.cpp
    src/connectivity/carplay_integration.cpp
    src/connectivity/android_auto_integration.cpp
)

target_link_libraries(toyota_connectivity
    toyota_multimedia_core
    Qt6::Network
    Qt6::DBus
)

# Navigation service
add_library(toyota_navigation SHARED
    src/navigation/navigation_engine.cpp
    src/navigation/map_renderer.cpp
    src/navigation/route_planner.cpp
    src/navigation/cloud_sync.cpp
)

target_link_libraries(toyota_navigation
    toyota_multimedia_core
    Qt6::Network
    Qt6::Positioning
)

# Media player
add_library(toyota_media SHARED
    src/media/audio_player.cpp
    src/media/video_player.cpp
    src/media/radio_tuner.cpp
    src/media/media_library.cpp
)

target_link_libraries(toyota_media
    toyota_multimedia_core
    Qt6::Multimedia
    ${GSTREAMER_LIBRARIES}
    ${PULSEAUDIO_LIBRARIES}
)

# Vehicle interface (CAN bus)
if(ENABLE_CAN)
    add_library(toyota_vehicle_interface SHARED
        src/vehicle-interface/can_interface.cpp
        src/vehicle-interface/vehicle_state.cpp
        src/vehicle-interface/diagnostic_interface.cpp
    )
    
    target_link_libraries(toyota_vehicle_interface
        toyota_multimedia_core
        ${SOCKETCAN_LIBRARIES}
    )
endif()

# HMI Application
if(BUILD_HMI)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    
    qt6_add_executable(toyota_multimedia_hmi
        src/hmi/main.cpp
        src/hmi/application.cpp
        src/hmi/qml.qrc
    )
    
    target_link_libraries(toyota_multimedia_hmi PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Qml
        toyota_multimedia_core
        toyota_connectivity
        toyota_navigation
        toyota_media
    )
    
    if(ENABLE_OTA)
        target_link_libraries(toyota_multimedia_hmi PRIVATE toyota_ota)
    endif()
    
    if(ENABLE_CAN)
        target_link_libraries(toyota_multimedia_hmi PRIVATE toyota_vehicle_interface)
    endif()
    
    # QML module
    qt6_add_qml_module(toyota_multimedia_hmi
        URI Toyota.Multimedia
        VERSION 1.0
        QML_FILES
            src/hmi/Main.qml
            src/hmi/HomeScreen.qml
            src/hmi/NavigationScreen.qml
            src/hmi/MediaScreen.qml
            src/hmi/PhoneScreen.qml
            src/hmi/SettingsScreen.qml
    )
endif()

# Testing
if(ENABLE_TESTING)
    enable_testing()
    find_package(GTest REQUIRED)
    
    add_executable(unit_tests
        tests/unit/test_ota_manager.cpp
        tests/unit/test_configuration.cpp
        tests/unit/test_security.cpp
        tests/unit/test_vehicle_interface.cpp
    )
    
    target_link_libraries(unit_tests
        GTest::GTest
        GTest::Main
        toyota_multimedia_core
        toyota_ota
        toyota_vehicle_interface
    )
    
    add_test(NAME UnitTests COMMAND unit_tests)
endif()

# Installation rules
install(TARGETS toyota_multimedia_core toyota_ota toyota_connectivity 
                toyota_navigation toyota_media
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

if(ENABLE_CAN)
    install(TARGETS toyota_vehicle_interface
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()

if(BUILD_HMI)
    install(TARGETS toyota_multimedia_hmi
        RUNTIME DESTINATION bin
    )
endif()

# Install headers
install(DIRECTORY include/toyota
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Systemd service files
install(FILES 
    deployment/systemd/toyota-multimedia.service
    deployment/systemd/toyota-ota.service
    DESTINATION /usr/lib/systemd/system
)

# Configuration files
install(FILES
    config/multimedia.conf
    config/ota.conf
    DESTINATION /etc/toyota
)

# CPack configuration for packaging
set(CPACK_PACKAGE_NAME "toyota-multimedia-system")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Toyota Motor Corporation")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Multimedia Systems Team <multimedia@toyota.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt6-base, gstreamer1.0, pulseaudio")
set(CPACK_RPM_PACKAGE_REQUIRES "qt6-qtbase, gstreamer1, pulseaudio")

include(CPack)

# Print build configuration
message(STATUS "")
message(STATUS "Toyota Multimedia System - Build Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Qt Version: ${Qt6_VERSION}")
message(STATUS "")
message(STATUS "Build Options:")
message(STATUS "  Testing: ${ENABLE_TESTING}")
message(STATUS "  Address Sanitizer: ${ENABLE_ASAN}")
message(STATUS "  Coverage: ${ENABLE_COVERAGE}")
message(STATUS "  HMI: ${BUILD_HMI}")
message(STATUS "  Services: ${BUILD_SERVICES}")
message(STATUS "")
message(STATUS "Features:")
message(STATUS "  CAN Bus: ${ENABLE_CAN}")
message(STATUS "  OTA Updates: ${ENABLE_OTA}")
message(STATUS "  CarPlay: ${ENABLE_CARPLAY}")
message(STATUS "  Android Auto: ${ENABLE_ANDROID_AUTO}")
message(STATUS "")
