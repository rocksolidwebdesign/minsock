REM Longhand:
REM
REM cl/nologo /c /D"MINSOCK_WINDOWS" /D"MINSOCK_EXPORT" "minsock.c"
REM cl/nologo /c /D"MINSOCK_WINDOWS" "main.c"
REM
REM link/nologo /dll "minsock.obj" /out:"minsock.dll"
REM link/nologo "main.obj" "minsock.lib"

cd build

cl/nologo /I"../include" /D"MINSOCK_DLL" /D"MINSOCK_EXPORT" "../minsock.c" /link /out:"minsock.dll" /DLL "ws2_32.lib"
cl/nologo /I"../include" /D"MINSOCK_DLL" /D"MINSOCK_EXPORT" "../minsock.cpp"  "minsock.lib" /link /out:"minsockxx.dll" /DLL "ws2_32.lib"

cl/nologo /I"../include" /D"MINSOCK_DLL" "../test/server.c"   "minsock.lib" /link /out:"server.exe"  "ws2_32.lib"
cl/nologo /I"../include" /D"MINSOCK_DLL" "../test/client.c"   "minsock.lib" /link /out:"client.exe" "ws2_32.lib"

cl/nologo /I"../include" /D"MINSOCK_DLL" "../test/client.cpp" "minsock.lib" "minsockxx.lib" /link /out:"clientxx.exe" "ws2_32.lib"
cl/nologo /I"../include" /D"MINSOCK_DLL" "../test/server.cpp" "minsock.lib" "minsockxx.lib" /link /out:"serverxx.exe" "ws2_32.lib"

cd ..

mv "build/minsock.dll" "bin"
mv "build/minsockxx.dll" "bin"

mv "build/server.exe" "bin"
mv "build/client.exe" "bin"

mv "build/serverxx.exe" "bin"
mv "build/clientxx.exe" "bin"
