@echo off
cl /nologo /TC /D _WINDOWS /I code /I libs/windows/include/SDL2 /I libs/lua code\common\*.c code\video\*.c code\sys\*.c code\audio\*.c code\unzip\*.c code\zlib\*.c /MD /link libs/windows/lib/*.lib /SUBSYSTEM:CONSOLE /OUT:io-lua.exe
del /q *.obj
if not exist build mkdir build
copy libs\windows\dll\*.dll build
copy game.zip build
copy base.zip build
move io-lua.exe build
