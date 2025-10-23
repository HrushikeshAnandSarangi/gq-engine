cmake_minimum_required(VERSION 3.5)
project(gq_engine CXX)

# Set the C++ standard directly
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and link the Drogon framework
find_package(Drogon CONFIG REQUIRED)

# Automaticd all source files in the 'controllers' and 'models' folders
aux_source_directory(controllers CTL_SRC)
aux_source_directory(models MODEL_SRC)

# Create the executable from main.cc
add_executable(${PROJECT_NAME} main.cc)

# Add the controller and model source files to the executable
target_sources(${PROJECT_NAME}
    PRIVATE
    ${CTL_SRC}
    ${MODEL_SRC}
)

# Tell the compiler where to find header files (like "models/Orders.h")
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link our executable against the Drogon library
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
