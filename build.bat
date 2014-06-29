@echo off
cl /nologo /TC /D _WINDOWS /D __WIN32__ /I static\windows /I code\common /I code\audio /I code\video /I code\libs\SDL2 /I code\libs\SDL2_ttf /I code\libs\SDL2_image /I code\libs\SDL2_mixer /I code\libs\lua /I code\libs\unzip /I code\libs\zlib code\common\*.c code\video\*.c code\sys\*.c code\audio\*.c code\libs\unzip\*.c /MD /link static\windows\*.lib /SUBSYSTEM:CONSOLE /OUT:io-lua.exe
del /q *.obj
if not exist build mkdir build
copy static\windows\*.dll build
copy game.zip build
copy base.zip build
move io-lua.exe build
