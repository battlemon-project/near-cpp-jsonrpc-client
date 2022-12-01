set GRPC_PATH_PLUGIN=J:\source\GitRepos\battlemon-project\grpc\install_Win64Release\bin
IF NOT EXIST GRPC_PATH_PLUGIN (goto :setPATH) else (goto :okPATH)
:setPATH
SET /P GRPC_PATH_PLUGIN="GRPC_PATH_PLUGIN: "
cd packages/protocol
#git pull origin master
cd ../..

:okPATH
cd NearPlugin & md protocol
cd protocol
del *.cc & del *.h
cd ../..

%GRPC_PATH_PLUGIN%/protoc.exe -I=%cd%/packages/protocol/ ^
--cpp_out=%cd%/NearPlugin/protocol/ %cd%/packages/protocol/*.proto

::proto.grpc.h generate
%GRPC_PATH_PLUGIN%/protoc.exe -I=%cd%/packages/protocol/  ^
--grpc_out=%cd%/NearPlugin/protocol/ ^
--plugin=protoc-gen-grpc="%GRPC_PATH_PLUGIN%/grpc_cpp_plugin.exe" %cd%/packages/protocol/*.proto
pause