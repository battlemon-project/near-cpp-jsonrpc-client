# near-cpp-jsonrpc-client
near-jsonrpc-client-rs DLL to use in cpp application

### MacOS
```
$ git clone https://github.com/grpc/grpc.git
$ cd grpc
$ git submodule update --init
$ mkdir -p cmake/build
$ cd cmake/build
$ cd cmake/build

$ export UE_THIRD_PARTY_DIR="./../UE_4.27/Engine/Source/ThirdParty"
$ export CMAKE_INSTALL_DIR="./../grpc/cmake/build/libs"

$ cmake \
 -DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_DIR" \
 -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 \
 -DCMAKE_OSX_ARCHITECTURES="x86_64" \
 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES=Release -Dprotobuf_BUILD_TESTS=OFF  \
 -DABSL_PROPAGATE_CXX_STD=ON \
 -DgRPC_ZLIB_PROVIDER=package \
 -DZLIB_INCLUDE_DIR="$UE_THIRD_PARTY_DIR/zlib/v1.2.8/include/Mac/" \
 -DZLIB_LIBRARY_DEBUG="$UE_THIRD_PARTY_DIR/zlib/v1.2.8/lib/Mac/libz.a" \
 -DZLIB_LIBRARY_RELEASE="$UE_THIRD_PARTY_DIR/zlib/v1.2.8/lib/Mac/libz.a" \
 -DgRPC_SSL_PROVIDER=package -DOPENSSL_ROOT_DIR="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k"\
 -DOPENSSL_INCLUDE_DIR="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/include" \
 -DOPENSSL_LIBRARIES_DIR="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/lib"  \
 -DLIB_EAY_LIBRARY_DEBUG="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/lib/libibcrypto.a"  \ -DLIB_EAY_LIBRARY_RELEASE="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/lib/libcrypto.a" \
 -DLIB_EAY_DEBUG="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/lib/libcrypto.a" \
 -DLIB_EAY_RELEASE="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/lib/libcrypto.a" \
 -DOPENSSL_INCLUDE_DIR="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/include/" \
 -DSSL_EAY_DEBUG="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/lib/libssl.a" \
 -DSSL_EAY_LIBRARY_DEBUG="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/lib/libssl.a" \
 -DSSL_EAY_LIBRARY_RELEASE="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/lib/libssl.a" \
 -DSSL_EAY_RELEASE="$UE_THIRD_PARTY_DIR/OpenSSL/1.1.1k/lib/libssl.a" \
 -G Xcode \
 ..
$ cmake --build . --target ALL_BUILD --config Release
$ cmake --install .
$ cd ../../..
$ mkdir build
$ cd build

$ cmake \
  -DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_DIR" \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 \
  -DCMAKE_OSX_ARCHITECTURES="x86_64" \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES=Release -Dprotobuf_BUILD_TESTS=OFF  \
  -DABSL_PROPAGATE_CXX_STD=ON \
  -G Xcode \
  ..
```

### Windows
```
run updateForWin.bat
