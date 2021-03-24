cmake ./CMakeLists.txt -G "MinGW Makefiles"
mingw32-make

xcopy "%~dp0\algo.exe" "%~dp0\cmake-build-debug" /Y

RMDIR "%~dp0\CMakeFiles" /S /Q
del "%~dp0\algo.exe"
del "%~dp0\cmake_install.cmake"
del "%~dp0\CMakeCache.txt"
del "%~dp0\Makefile"