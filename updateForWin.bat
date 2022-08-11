git submodule update --init
cd NearPlugin/src
xcopy /y /d "src" "."
RD /s /Q src
del ed25519_32.dll & del ed25519_64.dll
cd ../../packages
RD /s /Q grpc
git clone https://github.com/grpc/grpc.git
cd grpc & git submodule update --init

SET /P UE_THIRD_PARTY_DIR="Enter path: UE_4.27\Engine\Source\ThirdParty: "
SET /P GRPCARCHITECTURE="Enter ARCH Win32 or Win64: "

md .build%GRPCARCHITECTURE% & cd .build%GRPCARCHITECTURE%

cmake .. -G "Visual Studio 16 2019" -DARCHITECTURE=%GRPCARCHITECTURE% ^
    -DCMAKE_CXX_STANDARD_LIBRARIES="Crypt32.Lib User32.lib Advapi32.lib" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_CONFIGURATION_TYPES=Release ^
    -Dprotobuf_BUILD_TESTS=OFF ^
    -DgRPC_ZLIB_PROVIDER=package ^
    -DZLIB_INCLUDE_DIR="%UE_THIRD_PARTY_DIR%\zlib\v1.2.8\include\Win64\VS2015" ^
    -DZLIB_LIBRARY_DEBUG="%UE_THIRD_PARTY_DIR%\zlib\v1.2.8\lib\Win64\VS2015\Release\zlibstatic.lib" ^
    -DZLIB_LIBRARY_RELEASE="%UE_THIRD_PARTY_DIR%\zlib\v1.2.8\lib\Win64\VS2015\Release\zlibstatic.lib" ^
    -DgRPC_SSL_PROVIDER=package ^
    -DLIB_EAY_LIBRARY_DEBUG="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1k\Lib\Win64\VS2015\Release\libcrypto.lib" ^
    -DLIB_EAY_LIBRARY_RELEASE="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1k\Lib\Win64\VS2015\Release\libcrypto.lib" ^
    -DLIB_EAY_DEBUG="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1k\Lib\Win64\VS2015\Release\libcrypto.lib" ^
    -DLIB_EAY_RELEASE="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1k\Lib\Win64\VS2015\Release\libcrypto.lib" ^
    -DOPENSSL_INCLUDE_DIR="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1k\include\Win64\VS2015" ^
    -DSSL_EAY_DEBUG="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1k\Lib\Win64\VS2015\Release\libssl.lib" ^
    -DSSL_EAY_LIBRARY_DEBUG="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1k\Lib\Win64\VS2015\Release\libssl.lib" ^
    -DSSL_EAY_LIBRARY_RELEASE="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1k\Lib\Win64\VS2015\Release\libssl.lib" ^
    -DSSL_EAY_RELEASE="%UE_THIRD_PARTY_DIR%\OpenSSL\1.1.1k\Lib\Win64\VS2015\Release\libssl.lib"
cmake --build . --target ALL_BUILD --config Release