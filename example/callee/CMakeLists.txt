set(SRC_LIST main.cc ../caller/user.pb.cc ../caller/friend.pb.cc)

add_executable(provider ${SRC_LIST})
#mprpc与protobuf同等级，都是框架动态库
target_link_libraries(provider mprpc protobuf)