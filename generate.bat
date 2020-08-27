@echo off

echo Updating submodules...
git submodule update --init --remote

call "tools/premake5.exe" vs2019