@echo off
cl /nologo /TC /D _WINDOWS /I code /I windows/include/lua /I windows/include/SDL2 code\game\*.c code\common\*.c code\input\*.c code\video\*.c code\sys\*.c code\audio\*.c /MD /link windows/libs/*.lib  /SUBSYSTEM:CONSOLE /OUT:io-lua.exe
del /q *.obj
if not exist build mkdir build
copy windows\libs\*.lib build
copy windows\libs\*.dll build
mkdir build\base
robocopy base build\base /E >nul
move io-lua.exe build
