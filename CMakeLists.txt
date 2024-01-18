cmake_minimum_required(VERSION 3.5)

project(dbus)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find required Qt modules
find_package(Qt5 COMPONENTS Core DBus REQUIRED)

# Add the source files
set(SOURCES
    main.cpp
)

# Add the executable
add_executable(dbus ${SOURCES})

# Link Qt modules
target_link_libraries(dbus PRIVATE Qt5::Core Qt5::DBus)

# Set the include directories
target_include_directories(dbus PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add the CMakeLists.txt file for the MasterSlaveDBus subdirectory
add_subdirectory(MasterSlaveDBus)

# Link the MasterSlaveDBus library to the dbus executable
target_link_libraries(dbus PRIVATE MasterSlaveDBus)

# Install rules
install(TARGETS dbus
    DESTINATION /opt/dbusTest/bin
)
