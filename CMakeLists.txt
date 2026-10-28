cmake_minimum_required(VERSION 3.14)
project(FaceRecProject)

# Use C++ 17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV package installed via Homebrew
# This looks in standard paths like /opt/homebrew or /usr/local
find_package(OpenCV REQUIRED)

# Include OpenCV headers (so #include <opencv2/...> works)
include_directories(${OpenCV_INCLUDE_DIRS})

# Look for source files in the "src" folder
file(GLOB_RECURSE SOURCES "src/*.cpp")

# --- Executable 1: The Training Tool ---
add_executable(TrainModel src/train_model.cpp)
target_link_libraries(TrainModel ${OpenCV_LIBS})

# --- Executable 2: The Camera App ---
add_executable(FaceRecApp src/main.cpp)
target_link_libraries(FaceRecApp ${OpenCV_LIBS})

# --- Executable 3: The Capture Tool ---
add_executable(CaptureTool src/capture_tool.cpp)
target_link_libraries(CaptureTool ${OpenCV_LIBS})

# Copy assets to the build folder automatically
# This ensures the app finds the XML file when you run it
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})