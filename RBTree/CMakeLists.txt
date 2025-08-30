    add_library(RBTree SHARED "${CMAKE_CURRENT_SOURCE_DIR}/RBTree.c")
    target_link_directories(RBTree INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")