@echo off

if not exist ..\build mkdir ..\build
pushd ..\build

set WARNING=-W4 -D_CRT_SECURE_NO_WARNINGS
call cl -std:c++17 -Od -Z7 -D_WIN32 %WARNING% -nologo -EHsc ..\src\*.cpp -Fe:ember.exe

popd
