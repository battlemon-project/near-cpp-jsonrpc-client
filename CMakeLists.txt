cmake_minimum_required (VERSION 3.5)

project ("NearRPC")

if(APPLE) 
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR x86_64)
    set(CMAKE_C_ARCHIVE_FINISH true)
    set(CMAKE_CXX_ARCHIVE_FINISH true)
endif()


set(SOURSCES
	#NearPlugin/Source.cpp
    NearPlugin/Client.cpp
    NearPlugin/EdKeys.cpp
	NearPlugin/Helper.cpp
    NearPlugin/src/add_scalar.c
    NearPlugin/src/fe.c
    NearPlugin/src/ge.c
    NearPlugin/src/key_exchange.c
    NearPlugin/src/keypair.c
    NearPlugin/src/sc.c
    NearPlugin/src/seed.c
    NearPlugin/src/sha512.c
    NearPlugin/src/sign.c
    NearPlugin/src/verify.c
	NearPlugin/protocol/auth.pb.cc
    NearPlugin/protocol/auth.grpc.pb.cc
    NearPlugin/protocol/items.pb.cc
    NearPlugin/protocol/items.grpc.pb.cc
	NearPlugin/protocol/internalMm.pb.cc
    NearPlugin/protocol/internalMm.grpc.pb.cc
    NearPlugin/protocol/mm.pb.cc
    NearPlugin/protocol/mm.grpc.pb.cc
    NearPlugin/protocol/common.pb.cc
    NearPlugin/protocol/common.grpc.pb.cc
    NearPlugin/protocol/updates.grpc.pb.cc
    NearPlugin/protocol/updates.pb.cc
)
set(HEADERS
    NearPlugin/EdKeys.h
	NearPlugin/Helper.h
    NearPlugin/src/ed25519.h
    NearPlugin/src/fe.h
    NearPlugin/src/fixedint.h
    NearPlugin/src/ge.h
    NearPlugin/src/precomp_data.h
    NearPlugin/src/sc.h
    NearPlugin/src/sha512.h
	NearPlugin/protocol/auth.pb.h
    NearPlugin/protocol/auth.grpc.pb.h
    NearPlugin/protocol/items.pb.h
    NearPlugin/protocol/items.grpc.pb.h
	NearPlugin/protocol/internalMm.pb.h
    NearPlugin/protocol/internalMm.grpc.pb.h
    NearPlugin/protocol/mm.pb.h
    NearPlugin/protocol/mm.grpc.pb.h
    NearPlugin/protocol/common.pb.h
    NearPlugin/protocol/common.grpc.pb.h
    NearPlugin/protocol/updates.grpc.pb.h
    NearPlugin/protocol/updates.pb.h
	NearPlugin/include/Client.h
)
include_directories(NearPlugin)
include_directories(${CMAKE_INSTALL_PREFIX}/include/)

set(LIBS
)

if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/lib/llvm-11/include/c++/v1/ -stdlib=libc++ -pthread")
    add_link_options("-fuse-ld=lld")
endif()

add_library(NearRPC STATIC ${SOURSCES} ${HEADERS} ${LIBS})
target_link_libraries(NearRPC ${LIBS} -static-libgcc)

set_target_properties(
    NearRPC PROPERTIES
    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "YES"
    CXX_STANDARD 17
    CXX_EXTENSIONS ON
)