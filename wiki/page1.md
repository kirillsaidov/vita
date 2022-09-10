# Building and using Vita
To use Vita either copy and paste the source code into your project or link the Vita static library into your binary. You can [download](add a link) the precompiled static library or build it yourself.

## Requirements
* `CMake`

## Building
```
git clone https://gitlab.com/kirill.saidov/Vita.git
cd Vita && mkdir build
./build.sh
```
We clone the Vita repository, create a `build` directory and start the `build.sh` script that builds Vita into a static library. It will be located in the `lib` folder.

## Using Vita
Create a new project, lets call it `vita_test`, and copy the contents of `inc` folder into your project's folder. Copy `libvita.a` from `lib` as well. You should have the following folder structure:

```
- vita_test
    - libvita.a   // static library that we built
    - vita        // this is from 'inc' folder
    - main.c      // our source file
```

```C
// main.c
#include "vita/vita.h"

int32_t main(void) {
    const version_t v = get_vita_version();
    printf("Vita version: %s\n", v.str);

    return 0;
}
```

Let's build and run our project:
```
$ gcc main.c -o main -lvita -L.
$ ./main
Vita version: 0.3.0
```

## Debug vs Release builds
Vita comes in two variants: debug and release builds. It is highly recommended to use the debug-built Vita library for testing and debugging purposes and the release-built Vita library for your release builds.

There reason is simple: when using the debug version, your will benefit from internal checks done by Vita (asserts with error messages) that are removed in release builds. Nonetheless, you can still check the return value of a function to determine if an operation was successful. Almost every Vita functions returns a certain value.

Read more in [6. Debugging with Vita](page6.md).



