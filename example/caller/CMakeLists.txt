aux_source_directory(. SRC_LIST)

add_executable(rpccaller ${SRC_LIST})

target_link_libraries(rpccaller mprpc protobuf)

#target_link_libraries(rpccaller protobuf mprpc)
