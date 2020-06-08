cmake_minimum_required(VERSION 3.5)
project(VirtualizationCoreAPI)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)

# 查找当前目录下所有的源文件，并将名称保存到DIR_SRCS中
aux_source_directory(source DIR_SRCS)
aux_source_directory(protocol DIR_SRCS)

# 添加vca第三方的库
set(INC_DIR include)
set(INC_DIR /usr/include/python2.7)
set(LINK_DIR lib/libvbox-6.0.10/build)



include_directories(${INC_DIR})
link_directories(${LINK_DIR})

# 链接vca第三方库
link_libraries(python2.7)
link_libraries(vbox)
link_libraries(jsoncpp)
link_libraries(pthread)

# 链接protocol第三方库
link_libraries(protobuf)
link_libraries(grpc++)
link_libraries(grpc)
link_libraries(gpr)
link_libraries(z)
link_libraries(address_sorting)
#link_libraries(pthread)

add_executable(VirtualizationCoreAPI ${DIR_SRCS} source/vca_toCode.cpp include/vca_toCode.h
        include/vca_type.h source/vca_vmConfigure.cpp include/vca_vmConfigure.h
        include/vca_intType.h include/vca_event.h source/vca_event.cpp
        protocol/vca_controller.cpp protocol/vca_controller.h protocol/vca_handleType.h
        monitor/vca_monitorHook.h monitor/vca_monitorHook.cpp protocol/vca_enummethod.h protocol/vca_service.h murder/vca_murderHook.cpp murder/vca_murderHook.h)


target_link_libraries(VirtualizationCoreAPI -lvbox)
target_link_libraries(VirtualizationCoreAPI -lpython2.7)
target_link_libraries(VirtualizationCoreAPI -ljsoncpp)
target_link_libraries(VirtualizationCoreAPI -lpthread)
target_link_libraries(VirtualizationCoreAPI -lprotobuf)
target_link_libraries(VirtualizationCoreAPI -lgrpc++)
target_link_libraries(VirtualizationCoreAPI -lgrpc)
target_link_libraries(VirtualizationCoreAPI -lgpr)
target_link_libraries(VirtualizationCoreAPI -lz)
target_link_libraries(VirtualizationCoreAPI -laddress_sorting)
