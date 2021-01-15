@echo off

IF NOT EXIST .\build mkdir .\build
pushd .\build
cl -nologo -Zi -FC ../src/main.c -link User32.lib
popd
