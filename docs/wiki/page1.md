# Building `Vita` and project setup
To use `Vita` either copy and paste the source code into your project or link the `Vita` static library into your binary. You can [download](https://github.com/kirillsaidov/vita/releases) the precompiled static library or build it yourself.

## Requirements
* `C compiler (GCC, Clang, MinGW)`
* `CMake`

**NOTE:** on Windows you need the MSYS2 toolchain to use the `build.bat` script; or use your own alternative. 

## Building
```
git clone https://github.com/kirillsaidov/vita.git
cd vita && mkdir build
./build.sh  # linux and macos
./build.bat # windows
```
We clone the `Vita` repository, create a `build` directory and start the `build.sh` script that builds `Vita` into a static library. It will be located in the `lib` folder.

## Using `Vita`
Create a new project, lets call it `vita_test`, and copy the contents of `inc` folder into your project's folder. Copy `libvita.a` from `lib` as well. You should have the following folder structure:

```
- vita_test
    - libvita.a   // static library that we built
    - vita        // this is from 'inc' folder
    - main.c      // our source file
```

```c
// main.c
#include "vita/vita.h"

int32_t main(void) {
    const vt_version_t v = vt_version_get();
    printf("Vita version: %s\n", v.str);

    return 0;
}
```

Let's build and run our project:
```
$ gcc main.c -o main -lvita -L.
$ ./main
Vita version: 0.5.0
```

## Debug vs Release builds
By default `Vita` builds in **debug** mode. To build in **release** mode, open the `CMakeLists.txt` file and find two lines below, comment out the `Debug` version and uncomment the `Release` version:
```sh
# COMMENT OUT
# set(DEFAULT_BUILD_TYPE "Debug")
# set(CMAKE_C_FLAGS "-Wall -Wpedantic -Wextra -Wreturn-type -Wswitch -Wunused -Werror -O2")

# UNCOMMENT
set(DEFAULT_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS "-DNDEBUG -Wall -Wpedantic -Wextra -Wreturn-type -Wswitch -Wunused -Werror -O2")
```

`Vita` comes in two variants: debug and release builds. It is highly recommended to use the debug build for testing and debugging purposes and the release build `Vita` library for your finalized project. You will benefit from internal checks, assertions and debug error messages that are removed in release builds. Nevertheless, you can still check the return value of a function to determine if an operation was successful. Almost every `Vita` functions returns a certain value.

**[ [Back](VITA.md) | [Next](page2.md) ]**
