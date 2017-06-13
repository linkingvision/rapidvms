protoc -I=.\ --cpp_out=dllexport_decl=VE_LIBRARY_API:..\ .\vidconf.proto
protoc -I=.\ --cpp_out=dllexport_decl=VE_LIBRARY_API:..\ .\linkproto.proto
protoc -I=.\ --cpp_out=dllexport_decl=VE_LIBRARY_API:..\ .\linksystem.proto