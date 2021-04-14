:: Build Epilobe
mkdir ..\src\Epilobe\cmake\
cmake ..\src\Epilobe\CMakeLists.txt -G "MinGW Makefiles" -B ..\src\Epilobe\cmake\ -S ..\src\Epilobe 
mingw32-make -C ..\src\Epilobe\cmake

:: Build algo
mkdir ..\src\algo\cmake\
cmake ..\src\algo\CMakeLists.txt -G "MinGW Makefiles" -B ..\src\algo\cmake\ -S ..\src\algo 
mingw32-make -C ..\src\algo\cmake

:: Build Algo3
mkdir ..\Algo3\cmake\
cmake ..\Algo3\CMakeLists.txt -G "MinGW Makefiles" -B ..\Algo3\cmake\ -S ..\Algo3
mingw32-make -C ..\Algo3\cmake