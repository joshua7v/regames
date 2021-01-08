@echo off

IF NOT EXIST .\build mkdir .\build
pushd .\build
cl -nologo -FC ../src/main.c -link User32.lib
popd
