# Dev notes
This is a list of guidelines (my coding style) that I adhere to while developing Vita library.

#### Naming conventions used in Vita
1. variables: camelCase (including constants)
```
int helloWorld;     // local
int g_helloWorld    // global
int gi_helloWorld;  // global private (static)
int time_secs;      // append units of measurement at the end
```
2. structures: PascalCase
3. struct members: camelCase
```
struct BlackBox {
    int unknownBody;
    bool exists;
};
```
4. functions: module_lower_underscore_case
```
void module_function_name(struct BlackBox* blackBox);
```
5. enum: ePascalCase
6. enum members: eEnumName_CAPITALIZED
```
enum EWorldElements {
    EWorldElements_AIR,
    EWorldElements_WATER,
    EWorldElements_EARTH,
    EWorldElements_FIRE,
    EWorldElements_COUNT // to check how many elements does an enum contain
}
```
7. macros : mCAPITALIZED
```
#define mMAX(a, b) (((a)>(b)) ? (a) : (b)) // dont use a macro, if a function can be used instead
```

##### Naming
- `module` name must be prepended to every `function`.
- if a `struct` or an `enum` is typedef'ed, then `_t` must be appended at the end; if it's a pointer, then `_pt` should be appended instead.

#### Separating code into .h and .c
- private variables and functions should be marked as `static` and placed into .c file, both the declaration at the top of the file folllowed by its definition in the same order down below
- declarations of public variables and functions should be palced into .h and marked as `extern` and defined in .c file
- if you need to hide the `stuct` contents from the user, `struct` declaration should be placed into .h file and defition into .c file, otherwise, put everyhing in the .h file

#### Coding
* variables should be declared and initialized simoultaneously, even if you reinitialize them later; 
don't:
```
int a, b = 0, c;
float d;

// somewhere down below
a = b = c = 0;
d = 0;
```
instead do:
```
int a = 0, b = 0, c = 0;
float d = 0;
```
It seams so trivial, yet this is something people always forget to do, including myself (sometimes).

* braces: always use curly braces
don't:
```
if(condition)
    action
```
instead do:
```
if(condition) {
    action
}
```

* commets
for commenting out functions and in general use: `/* ... */` or `//...` for one-line comments
```
/* void some_func() {...} */
```
for documentation: `/** ... */`
```
/** some_func ==> what does it do (short description, for long description use: notes)
    params:
        param1
        param2
    returns:
        type (clarify here if needed; if the return type is void, the section should be omitted)
    notes or requires:
        additional information about the function, or requirements (if both are needed, seperate 
        them into different sections)
*/
void some_func() {...}
```

---
### References
If something is not specified, then read more at [C coding standard](https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html#brace).








