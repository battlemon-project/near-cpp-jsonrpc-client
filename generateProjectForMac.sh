#!/bin/sh
#Ed25519: update
read -p "SCRIPT_DIR: " ROOT_DIR
read -p "UE_4.27: " UE_DIR
read -p "Roote install libs: " CMAKE_INSTALL_DIR

cd $ROOT_DIR/NearPlugin
git clone https://github.com/orlp/ed25519.git
cp -r $ROOT_DIR/NearPlugin/ed25519/src $ROOT_DIR/NearPlugin/
Rm -R ed25519
cd $ROOT_DIR/packages

#gRPC: update
git clone --depth 1 --branch v1.49.1 https://github.com/grpc/grpc.git
cd grpc
git submodule update --init
mkdir -p cmake/build
cd cmake/build

#gRPC: generate progect
/Applications/CMake.app/Contents/bin/cmake \
 -DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_DIR" \
 -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 \
 -DCMAKE_OSX_ARCHITECTURES="x86_64" \
 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES=Release -Dprotobuf_BUILD_TESTS=OFF  \
 -DABSL_PROPAGATE_CXX_STD=ON \
 -DgRPC_ZLIB_PROVIDER=package \
 -DZLIB_INCLUDE_DIR="$UE_DIR/Engine/Source/ThirdParty/zlib/v1.2.8/include/Mac/" \
 -DZLIB_LIBRARY_DEBUG="$UE_DIR/Engine/Source/ThirdParty/zlib/v1.2.8/lib/Mac/libz.a" \
 -DZLIB_LIBRARY_RELEASE="$UE_DIR/Engine/Source/ThirdParty/zlib/v1.2.8/lib/Mac/libz.a" \
 -DgRPC_SSL_PROVIDER=package -DOPENSSL_ROOT_DIR="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k"\
 -DOPENSSL_INCLUDE_DIR="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/include/Mac" \
 -DOPENSSL_LIBRARIES_DIR="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/lib/Mac"  \
 -DLIB_EAY_LIBRARY_DEBUG="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/lib/Mac/libibcrypto.a"  \ -DLIB_EAY_LIBRARY_RELEASE="$UE_DIR/OpenSSL/1.1.1k/lib/Mac/libcrypto.a" \
 -DLIB_EAY_DEBUG="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/lib/Mac/libcrypto.a" \
 -DLIB_EAY_RELEASE="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/lib/Mac/libcrypto.a" \
 -DOPENSSL_INCLUDE_DIR="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/include/Mac" \
 -DSSL_EAY_DEBUG="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/lib/Mac/libssl.a" \
 -DSSL_EAY_LIBRARY_DEBUG="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/lib/Mac/libssl.a" \
 -DSSL_EAY_LIBRARY_RELEASE="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/lib/Mac/libssl.a" \
 -DSSL_EAY_RELEASE="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/lib/Mac/libssl.a" \
 -DOPENSSL_CRYPTO_LIBRARY="$UE_DIR/Engine/Source/ThirdParty/OpenSSL/1.1.1k/lib/Mac/libcrypto.a" \
 -G Xcode \
 ../..
 #gRPC: build progect
/Applications/CMake.app/Contents/bin/cmake --build . --target ALL_BUILD --config Release
/Applications/CMake.app/Contents/bin/cmake --install .

cd $ROOT_DIR

mkdir build
cd build

#NearCPP: generate progect
/Applications/CMake.app/Contents/bin/cmake \
  -DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_DIR" \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 \
  -DCMAKE_OSX_ARCHITECTURES="x86_64" \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES=Release  \
  -DABSL_PROPAGATE_CXX_STD=ON \
  -G Xcode \
  ..

cd $ROOT_DIR

export Arr[0]=auth.proto
export Arr[1]=common.proto
export Arr[2]=internalAuth.proto
export Arr[3]=internalItems.proto
export Arr[4]=internalMm.proto
export Arr[5]=items.proto
export Arr[6]=mm.proto
export Arr[7]=users.proto
export Arr[8]=websocket.proto


length=${#Arr[@]}
for (( j=0; j<${length}; j++ ));
do
$CMAKE_INSTALL_DIR/bin/protoc -I=$cd/packages/protocol/ \
--cpp_out=$cd/NearPlugin/protocol/ $cd/packages/protocol/${#Arr[j]}

::proto.grpc.h generate
$CMAKE_INSTALL_DIR/bin/protoc -I=$cd/packages/protocol/  \
--grpc_out=$cd/NearPlugin/protocol/ \
--plugin=protoc-gen-grpc="$CMAKE_INSTALL_DIR/bin/grpc_cpp_plugin" $cd/packages/protocol/${#Arr[j]}
done