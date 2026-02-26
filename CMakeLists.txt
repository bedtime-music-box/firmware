cmake_minimum_required(VERSION 3.16)

# Set the project name
set(PROJECT_NAME "bedtime_music_box")

# Load the ESP-IDF build system
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Define the project and set the version number
project(${PROJECT_NAME} 0.0.1)
