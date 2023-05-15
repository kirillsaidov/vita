@echo off

if not exist build ( 
    md build 
)

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=C:\msys64\mingw64\bin\gcc.exe -H%cd% -B%cd%/build -G "MinGW Makefiles"
cmake --build %cd%/build --config Debug



