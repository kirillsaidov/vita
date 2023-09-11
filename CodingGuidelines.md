# C coding guidelines (subjective)
This is a list of guidelines that I adhere to while developing Vita library.

## A few notes for future revision
* Don't use MACROS if a function can be used instead.
* Avoid ambiguity.
* There must be only one way to do something.

---

## Naming
### Variables
Use `camelCase` for variables. If needed, UPPER_CASE for const.

```c
int helloWorld;                 // local
int g_helloWorld                // global
int gi_helloWorld;              // global private (static)
int time_secs;                  // append units of measurement at the end
const int width/WIDTH = 320;    // const
#define HEIGHT = 160;           // defines
```

### Structs and Enums
Use `PascalCase` when naming structs and `camelCase` for struct members.

```c
// if that is a POD:
struct BlackBox {
    int unknownBody;
    bool exists;
};

// if that is an object (will behave like an object)
typedef struct BlackBox {
    int unknownBody;
    bool exists;

    int (*some_func)(void);
} bb_t;
```

Enums must be `ALL_CAPS`, sometimes `lower_underscore_case` style is also acceptable. 

```c
enum WorldElements { // => world_elements_enum_value
    WORLD_ELEMENTS_AIR,
    WORLD_ELEMENTS_WATER,
    WORLD_ELEMENTS_EARTH,
    WORLD_ELEMENTS_FIRE,
    WORLD_ELEMENTS_COUNT // number of elements
};
```

Don't `typedef` enums! 

### Functions
Project_Module/File name should be prepended to functions. The `project_` acts as a namespace to dissambiguate between other library functionality. 

```c
void project_module_function_name(struct BlackBox* blackBox);
```

### Macros
Use `UPPER_UNDERSCORE_CASE` for macros.

```c
// don't use a macro, if a function can be used instead
#define MAX(a, b) (((a)>(b)) ? (a) : (b))
```

### Typedefs
`struct` should be `typedef`'ed with `_t` appended at the end.

```c
typedef struct BlackBox {
    int box_id;
} bbox_t;
```

### Gotos
Labels should start with `prefix_label__` prefix and 2 underscores should be appended at the end.
```c
// this is a goto example:
vt_label_my_function_name__:
// do something ...
```

### Header guards
Always prepend project name to the header guard to avoid name collisions.

```c
#ifndef BLACKBOX_MYSTERY_H
#define BLACKBOX_MYSTERY_H

// In this case BLACKBOX is a project name and MYSTERY is a module name

#endif // BLACKBOX_MYSTERY_H
```

Or even better add package name as well:

```c
#ifndef BLACKBOX_SPACE_MYSTERY_H
#define BLACKBOX_SPACE_MYSTERY_H

// In this case BLACKBOX is a project name, SPACE is a package and MYSTERY is a module

#endif // BLACKBOX_SPACE_MYSTERY_H => project_package_module
```

---

## Separating code into .h and .c
* private variables and functions should be marked as `static` and placed into `.c` file, in the same order
- declarations of public variables and functions should be placed into `.h` and marked as `extern` and defined in `.c` file
- if you need to hide `struct` contents from the user, `struct` declaration should be placed into `.h` whilst the its definition into `.c` file

## Coding style

### Variable declaration
Always initialize variables upon declaration, even if you reinitialize them later;

```c
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
Always use curly braces (explicit scope `{...}` is better)

```c
// don't:
if (condition)
    action;

// do:
if (condition) {
    action;
}
```

#### Ternary operator
If a condition is short, place it on one line.

```c
(condition) ? statement : statement;
```

If its long, split it into multiple lines.

```c
(condition)
    ? a very long statement
    : another even longer statement;
```

#### Comments
* for commenting out functions use: `/* ... */` or `//...` for one-line comments.

```c
/* void some_func() {...} */
```

* for documentation: `/** ... */`

```c
/** Description
    @param param1 description
    @param param2 description

    @returns result

    @note
        additional information about the function,
        or requirements (if both are needed, seperate
        them into different sections)

    @bug
        Found some weird bug #112

    @see
        Check this out: link

    @todo
        Fix bug #112
*/
float some_func(int param1, int param2) {...}
```

## Conditional compilation
Use `#if` instead of `#ifdef`.

```c
// don't:
#ifdef DEBUG
    // debug code
#endif

// do:
#if defined(DEBUG)
    // debug code
#endif
```

When setting a macro variable outside a program, `#if` will always pick it up, `#ifdef` won't. 

```c
gcc ... -DEBUG=0
```

More over, the former allows for compound confiditons, where as the latter accepts only one condition. To test whether a symbol is defined use `#if defined(...)`:

```c
#if defined(_WIN32) || defined(_WIN64)
    // code
#endif
```

## Error checking
Remember to always use `assert` when checking for programmer errors. Otherwise, a control flow or user error should be handled accordingly with return codes (or a different method).

```c
void validate(const char *z) {
    assert(z != NULL); // programmer is responsible for passing a valid string
    
    // now do your thing
    // ...
}
```

---

### References
If something is not specified, then read more at [C coding standard](https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html#brace).
