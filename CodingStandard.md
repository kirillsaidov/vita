# C coding standard (subjective)
This is a list of guidelines that I adhere to while developing Vita library.

## Naming
#### Variables
Use `camelCase` for mutable variables and UPPER_CASE for const.

```C
int helloWorld;                 // local
int g_helloWorld                // global
int gi_helloWorld;              // global private (static)
int time_secs;                  // append units of measurement at the end
const int width/WIDTH = 320;    // const
#define HEIGHT = 160;           // defines
```

#### Structs and Enums
Use `PascalCase` when naming structs and `camelCase` for struct members.

```C
struct BlackBox {
    int unknownBody;
    bool exists;
};
```

Use capital letters of an enum as a prefix for enum members.

```C
enum WorldElements { // (W)orld (E)elements => we_enum_name
    we_air,
    we_water,
    we_earth,
    we_fire,
    we_count // number of elements
}
```

#### Functions
Module/File name should be prepended to functions.

```C
void module_function_name(struct BlackBox* blackBox);
```

#### Macros
Use `UPPER_UNDERSCORE_CASE` for macros.

```C
// don't use a macro, if a function can be used instead
#define MAX(a, b) (((a)>(b)) ? (a) : (b))
```

#### Typedefs
If `struct` or an `enum` is `typedef`'ed, then `_t` must be appended at the end; if it's a pointer, then `_ptr` should be appended instead.

```C
typedef struct BlackBox {
    int darkness_level;
} bbox_t;
```

#### Header guards
Always prepend project name to the header guard to avoid name collisions.

```C
#ifndef BLACKBOX_HEADER_H
#define BLACKBOX_HEADER_H

// In this case BLACKBOX is a project name

#endif // BLACKBOX_HEADER_H
```

## Separating code into .h and .c
* private variables and functions should be marked as `static` and placed into `.c` file, in the same order
- declarations of public variables and functions should be placed into `.h` and marked as `extern` and defined in `.c` file
- if you need to hide `struct` contents from the user, `struct` declaration should be placed into `.h` whilst the its definition into `.c` file

## Coding style

#### Variable declaration
Always initialize variables upon declaration, even if you reinitialize them later;

```C
// don't:
int a, b = 0, c;
float d;

a = b = c = 0;
d = 0;

// do:
int a = 0, b = 0, c = 0;
float d = 0;
```

#### Braces
Always use curly braces (explicit scope {} is better)

```C
// don't:
if(condition)
    action;

// do:
if(condition) {
    action;
}
```

#### Ternary operator
If a condition is short, place it on one line.

```C
(condition) ? statement : statement;
```

If its long, split it into multiple lines.

```C
(condition)
    ? a very long statement
    : another even longer statement;
```

#### Comments
* for commenting out functions use: `/* ... */` or `//...` for one-line comments.

```C
/* void some_func() {...} */
```

* for documentation: `/** ... */`

```C
/**
Description

Params:
    param1 = explanation
    param2 = explanation

Returns: result

Notes:
    additional information about the function,
    or requirements (if both are needed, seperate
    them into different sections)

Bug:
    Found some weird bug #112

See:
    Check this out: link

Todo:
    Fix bug #112
*/
float some_func(int param1, int param2) {...}
```

## Conditional compilation
Use `#if` instead of `#ifdef`.

```C
// don't:
#ifdef DEBUG
    // debug code
#endif

// do:
#if DEBUG
    // debug code
#endif
```

When setting a macro variable outside a program, `#if` will always pick it up, `#ifdef` won't.

```C
gcc ... -DEBUG=0
```

To test whether a symbol is defined use `#if defined(...)`:

```C
#if defined(WIN32)
    // code
#endif
```

---

### References
If something is not specified, then read more at [C coding standard](https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html#brace).
