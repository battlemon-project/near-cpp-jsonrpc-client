SET CD_DIR=J:\source\GitRepos\battlemon-project\near-cpp-jsonrpc-client
SET CMAKE_INSTALL_DIR=%CD_DIR%/libs
SET UE_THIRD_PARTY_DIR=J:\UnrealEngine-4.27.2-release\Engine\Source\ThirdParty

cd %CD_DIR%\packages\grpc
md build & cd build

cmake .. -G "Unix Makefiles" \
    -DCMAKE_INSTALL_PREFIX="%CMAKE_INSTALL_DIR%" \
    -DCMAKE_BUILD_TYPE=Release \
    -Dprotobuf_BUILD_TESTS=OFF \
    -DgRPC_ZLIB_PROVIDER=package \
    -DZLIB_INCLUDE_DIR="%UE_THIRD_PARTY_DIR%\zlib\v1.2.8\include\Linux\x86_64-unknown-linux-gnu" \
    -DZLIB_LIBRARY_DEBUG="%UE_THIRD_PARTY_DIR%\zlib\v1.2.8\Lib\Linux\x86_64-unknown-linux-gnu\libz.a" \
    -DZLIB_LIBRARY_RELEASE="%UE_THIRD_PARTY_DIR%\zlib\v1.2.8\Lib\Linux\x86_64-unknown-linux-gnu\libz.a" \
    -DgRPC_SSL_PROVIDER=package \
    -DOPENSSL_INCLUDE_DIR="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1c\include\Linux\x86_64-unknown-linux-gnu" \
    -DOPENSSL_SSL_LIBRARY="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1c\lib\Linux\x86_64-unknown-linux-gnu\libssl.a" \
    -DOPENSSL_CRYPTO_LIBRARY="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1c\lib\Linux\x86_64-unknown-linux-gnu\libcrypto.a" \
    -DCMAKE_C_COMPILER=J:\UnrealEngine-4.27.2-release\ClangCompiler\x86_64-unknown-linux-gnu\bin\clang -DCMAKE_CXX_COMPILER=J:\UnrealEngine-4.27.2-release\ClangCompiler\x86_64-unknown-linux-gnu\bin\clang++
cmake --build .
cmake --install .

cd %CD_DIR%
md build & cd build

cmake \
  -DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_DIR" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES=Release  \
  -G "Unix Makefiles" \
  ..
cmake --build .
