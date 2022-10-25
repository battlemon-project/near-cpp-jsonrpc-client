mkdir libs
export CD_DIR=/home/sia/project
export CMAKE_INSTALL_DIR=/home/sia/project/libs
export UE_THIRD_PARTY_DIR=/home/sia/project/UE4
cd /home/sia/project/packages
git clone https://github.com/grpc/grpc.git
cd grpc
git submodule update --init
mkdir build
cd build
cmake .. -G "Unix Makefiles" \
    -DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -Dprotobuf_BUILD_TESTS=OFF \
    -DgRPC_ZLIB_PROVIDER=package \
    -DZLIB_INCLUDE_DIR="$UE_THIRD_PARTY_DIR\zlib\v1.2.8\include\Linux\x86_64-unknown-linux-gnu" \
    -DZLIB_LIBRARY_DEBUG="$UE_THIRD_PARTY_DIR\zlib\v1.2.8\Lib\Linux\x86_64-unknown-linux-gnu\libz.a" \
    -DZLIB_LIBRARY_RELEASE="$UE_THIRD_PARTY_DIR\zlib\v1.2.8\Lib\Linux\x86_64-unknown-linux-gnu\libz.a" \
    -DgRPC_SSL_PROVIDER=package \
    -DOPENSSL_INCLUDE_DIR="$UE_THIRD_PARTY_DIR\OpenSSL\1.1.1c\include\Linux\x86_64-unknown-linux-gnu" \
    -DOPENSSL_SSL_LIBRARY="$UE_THIRD_PARTY_DIR\OpenSSL\1.1.1c\lib\Linux\x86_64-unknown-linux-gnu\libssl.a" \
    -DOPENSSL_CRYPTO_LIBRARY="$UE_THIRD_PARTY_DIR\OpenSSL\1.1.1c\lib\Linux\x86_64-unknown-linux-gnu\libcrypto.a" \
    -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build .
cmake --install .
cd $CD_DIR
$CMAKE_INSTALL_DIR/bin/protoc -I=$CD_DIR/packages/protocol/ \
--cpp_out=$CD_DIR/NearPlugin/protocol/ $CD_DIR/packages/protocol/auth.proto
::proto.grpc.h generate
$CMAKE_INSTALL_DIR/bin/protoc -I=$CD_DIR/packages/protocol/  \
--grpc_out=$CD_DIR/NearPlugin/protocol/ \
--plugin=protoc-gen-grpc="$CMAKE_INSTALL_DIR/bin/grpc_cpp_plugin" $CD_DIR/packages/protocol/auth.proto
$CMAKE_INSTALL_DIR/bin/protoc -I=$CD_DIR/packages/protocol/ \
--cpp_out=$CD_DIR/NearPlugin/protocol/ $CD_DIR/packages/protocol/common.proto
::proto.grpc.h generate
$CMAKE_INSTALL_DIR/bin/protoc -I=$CD_DIR/packages/protocol/  \
--grpc_out=$CD_DIR/NearPlugin/protocol/ \
--plugin=protoc-gen-grpc="$CMAKE_INSTALL_DIR/bin/grpc_cpp_plugin" $CD_DIR/packages/protocol/common.proto
$CMAKE_INSTALL_DIR/bin/protoc -I=$CD_DIR/packages/protocol/ \
--cpp_out=$CD_DIR/NearPlugin/protocol/ $CD_DIR/packages/protocol/items.proto
::proto.grpc.h generate
$CMAKE_INSTALL_DIR/bin/protoc -I=$CD_DIR/packages/protocol/  \
--grpc_out=$CD_DIR/NearPlugin/protocol/ \
--plugin=protoc-gen-grpc="$CMAKE_INSTALL_DIR/bin/grpc_cpp_plugin" $CD_DIR/packages/protocol/items.proto
mkdir build
cd build
cmake \
  -DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_DIR" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES=Release  \
  -G "Unix Makefiles" \
  ..
cmake --build .
