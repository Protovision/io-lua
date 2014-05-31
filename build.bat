@echo off
cl /nologo /TC /D _WINDOWS /I code /I libs/windows/include/SDL2 /I libs/lua code\common\*.c code\video\*.c code\sys\*.c code\audio\*.c /MD /link libs/windows/lib/*.lib /SUBSYSTEM:CONSOLE /OUT:io-lua.exe
del /q *.obj
if not exist build mkdir build
copy libs\windows\dll\*.dll build
mkdir build\game
robocopy game build\game /E >nul
mkdir build\base
robocopy base build\base /E >nul
move io-lua.exe build
