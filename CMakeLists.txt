cmake_minimum_required(VERSION 3.17)
project(protoOS)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(resources)
include_directories(resources/Example)
include_directories(resources/Example/Protogen_OS_V3.1_mod)
include_directories(resources/WiichuckDemo)
include_directories(vb0.1)
include_directories(vb0.2)
include_directories(vb1.0)
include_directories(vb1.0/led_test)
include_directories(vb1.1)
include_directories(vb1.2)
include_directories(vb1.3)
include_directories(vb1.4)
include_directories(vb2.0)
include_directories(vb3.0)

add_executable(protoOS
        resources/Example/Protogen_OS_V3.1_mod/Protogen_OS_V3.1_mod.ino
        resources/WiichuckDemo/nunchuck_funcs.h
        resources/WiichuckDemo/WiichuckDemo.ino
        vb0.1/nunchuck_funcs.h
        vb0.1/vb0.1.ino
        vb0.2/nunchuck_funcs.h
        vb0.2/vb0.2.ino
        vb1.0/led_test/led_test.ino
        vb1.0/nunchuck_funcs.h
        vb1.0/vb1.0.ino
        vb1.1/nunchuck_funcs.h
        vb1.1/vb1.1.ino
        vb1.2/nunchuck_funcs.h
        vb1.2/vb1.2.ino
        vb1.3/nunchuck_funcs.h
        vb1.3/vb1.3.ino
        vb1.4/materials.md
        vb1.4/nunchuck_funcs.h
        vb1.4/vb1.4.ino
        vb2.0/nunchuck_funcs.h
        vb2.0/vb2.0.ino
        vb3.0/vb3.0.ino
        LICENSE
        README.md)
