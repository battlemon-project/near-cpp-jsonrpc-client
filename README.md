# near-cpp-jsonrpc-client
near-jsonrpc-client-rs DLL to use in cpp application

### MacOS
```
$ sh generateProjectForMac.sh
```
Open near-cpp-jsonrpc-client/build/NearRPC.xcodeproj;
Build Rules->add new rule-> Process: LLVM, Using: Apple Clang ;
Build Phases->Add link binary with libraries
Build project
### Windows
```
> updateForWin.bat
```
Open .sln and build project
