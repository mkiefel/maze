set (builder_srcs builder_application.cc)
add_executable (builder ${builder_srcs})

target_link_libraries (builder ${PNG_LIBRARIES})
target_link_libraries (builder ${Boost_LIBRARIES})
