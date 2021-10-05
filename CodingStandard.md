# C coding standard (subjective)
This is a list of guidelines that I adhere to while developing Vita library.

#### Naming conventions used in Vita
1. variables: camelCase (mutable), UPPER_CASE (const)
```C
int helloWorld;         // local
int g_helloWorld        // global
int gi_helloWorld;      // global private (static)
int time_secs;          // append units of measurement at the end
const WIDTH = 320;      // const
#define HEIGHT = 160;   // defines
```
2. structs: PascalCase
3. struct members: camelCase
```C
struct BlackBox {
    int unknownBody;
    bool exists;
};
```
4. functions: module_lower_underscore_case
```C
void module_function_name(struct BlackBox* blackBox);
```
5. enum: `P`ascal`C`ase => pc (use capital letters as prefix)
6. enum members: pc_member_name,
```C
enum WorldElements {
    we_air,
    we_water,
    we_earth,
    we_fire,
    we_count // number of elements
}
```
7. macros : UPPER_UNDERSCORE_CASE
```C
#define MAX(a, b) (((a)>(b)) ? (a) : (b)) // dont use a macro, if a function can be used instead
```

##### Naming
- `module` name must be prepended to every `function`.
- if a `struct` or an `enum` is `typedef`'ed, then `_t` must be appended at the end; if it's a pointer, then `_ptr` should be appended instead.

#### Separating code into .h and .c
- private variables and functions should be marked as `static` and placed into `.c` file, both the declaration at the top of the file folllowed by its definition in the same order down below
- declarations of public variables and functions should be palced into `.h` and marked as `extern` and defined in `.c` file
- if you need to hide the `stuct` contents from the user, `struct` declaration should be placed into `.h` file and definition into `.c` file

#### Coding
* variables should be declared and initialized simoultaneously, even if you reinitialize them later; 
don't:
```C
int a, b = 0, c;
float d;

// somewhere down below
a = b = c = 0;
d = 0;
```
instead do:
```C
int a = 0, b = 0, c = 0;
float d = 0;
```
* braces: always use curly braces (explicit scope {} is better)
don't:
```C
if(condition)
    action;
```
instead do:
```C
if(condition) {
    action;
}
```

* commets
for commenting out functions and in general use: `/* ... */` or `//...` for one-line comments
```C
/* void some_func() {...} */
```
for documentation: `/** ... */`
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
*/
float some_func(int param1, int param2) {...}
```

---
### References
If something is not specified, then read more at [C coding standard](https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html#brace).








