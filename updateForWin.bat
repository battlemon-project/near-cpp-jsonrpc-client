::Ed25519: update
git submodule update --init
cd NearPlugin/src
xcopy /y /d "src" "."
RD /s /Q src
del ed25519_32.dll & del ed25519_64.dll
cd ../../packages/grpc
git submodule update --init

::gRPC: generate progect
SET /P UE_THIRD_PARTY_DIR="Enter path: UE_4.27\Engine\Source\ThirdParty: "
SET /P GRPCARCHITECTURE="Enter ARCH Win32 or Win64: "

md install_%GRPCARCHITECTURE%Debug
SET CMAKE_INSTALL_DIR_P=%cd%/install_%GRPCARCHITECTURE%Debug

md .build%GRPCARCHITECTURE%Debug
cd .build%GRPCARCHITECTURE%Debug

cmake .. -G "Visual Studio 16 2019" -DARCHITECTURE=%GRPCARCHITECTURE% ^
    -DCMAKE_CXX_STANDARD_LIBRARIES="Crypt32.Lib User32.lib Advapi32.lib" ^
	-DCMAKE_INSTALL_PREFIX="%CMAKE_INSTALL_DIR_P%" ^
    -DCMAKE_BUILD_TYPE=Debug ^
    -DCMAKE_CONFIGURATION_TYPES=Debug ^
    -Dprotobuf_BUILD_TESTS=OFF
	
cmake --build . --target ALL_BUILD --config Debug
cmake --install . --config Debug

cd ..

md install_%GRPCARCHITECTURE%Release
SET CMAKE_INSTALL_DIR_P=%cd%/install_%GRPCARCHITECTURE%Release
md .build%GRPCARCHITECTURE%Release
cd .build%GRPCARCHITECTURE%Release

cmake .. -G "Visual Studio 16 2019" -DARCHITECTURE=%GRPCARCHITECTURE% ^
    -DCMAKE_CXX_STANDARD_LIBRARIES="Crypt32.Lib User32.lib Advapi32.lib" ^
	-DCMAKE_INSTALL_PREFIX="%CMAKE_INSTALL_DIR_P%" ^
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
cmake --install . --config Release

::proto.h generate
cd ../../..

updateProtoHeadersCPP.bat
