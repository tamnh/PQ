cmake_minimum_required (VERSION 2.6)
project (CS5234)
# The version number.
set (CS5234 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "${PROJECT_SOURCE_DIR}/BufferTreeNode.cpp"
        "${PROJECT_BINARY_DIR}/LeafNode.h"
)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")