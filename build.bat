@echo off
echo Building Zweek Compiler...
mkdir build
cd build
cmake ..
cmake --build . --config Release
cd ..
echo.
echo Build complete. To run the compiler:
echo .\build\Release\zweek.exe hello.zw
