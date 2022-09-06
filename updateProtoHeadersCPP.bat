set Arr[0]=auth.proto
set Arr[1]=common.proto
set Arr[2]=internalAuth.proto
set Arr[3]=internalItems.proto
set Arr[4]=internalMm.proto
set Arr[5]=items.proto
set Arr[6]=mm.proto
set Arr[7]=users.proto
set Arr[8]=websocket.proto
set "x=0"

if not defined GRPCARCHITECTURE (goto :setWin) else (goto :okWin)
:setWin
SET /P GRPCARCHITECTURE="Enter ARCH Win32 or Win64: "
cd packages/protocol
git pull origin master
cd ../..

:okWin
cd NearPlugin/protocol
del *.cc & del *.h
cd ../..



:Loop
if not defined Arr[%x%] goto :endLoop

call set VAL=%%Arr[%x%]%%
%cd%/packages/grpc/.build%GRPCARCHITECTURE%/third_party/protobuf/Release/protoc.exe -I=%cd%/packages/protocol/ ^
--cpp_out=%cd%/NearPlugin/protocol/ %cd%/packages/protocol/%VAL%

::proto.grpc.h generate
%cd%/packages/grpc/.build%GRPCARCHITECTURE%/third_party/protobuf/Release/protoc.exe -I=%cd%/packages/protocol/  ^
--grpc_out=%cd%/NearPlugin/protocol/ ^
--plugin=protoc-gen-grpc="%cd%/packages/grpc/.build%GRPCARCHITECTURE%/Release/grpc_cpp_plugin.exe" %cd%/packages/protocol/%VAL%
SET /a "x+=1"
goto :Loop

:endLoop
pause