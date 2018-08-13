# Chapter 2. Variable and Basic Types

## 2.2 Variables


The value of an object of built-in type that is not explicitly initialized depends on where it is defined. Variables defined outside any function body are initialized to zero, while inside a function are uninitialized.

```cpp
std::string global_Str;
int global_Str;
int main(){
	int local_int;
	std::string local_str
}
```

`global_str` is global variable, so the value is empty string. `global_int` is global variable, so the value is zero. `local_int` is a local variable which is uninitialized, so it has a undefined value. `local_str` is also a local variable which is uninitialized, but it has a value that is defined by the class. So it is empty string. 


## 2.3 Compound Types

### 2.3.1 References

Once initialized, a reference remain bound to its initial object. There is no way to rebind a reference, hence reference must be initialized.

Reference is not an object, it is just another name for an already existing object.

```cpp
int &r = i, r2 = i2 // r is a reference bound to i, r2 is an int
int &refVal2;       // error: a reference must be initialized
int &refVal4 = 10;  // error: initializer must be an object
```

### 2.3.2 Pointers

Unlike reference, a pointer is an object in its own right. A pointer can point to several different objects over its lifetime. It does not need to be initialized at the time it is defined.

```cpp
double dp, *dp2;  // dp2 is a pointer to double; dp is a double
int *p = &ival;   // p holds the address of ival; p is a pointer to ival

int ival = 42;
int *p = &ival;   // p is a pointer to ival
cout << *p;       // * yields the object to which p points; print 42;
*p = 0;           // * yields the object; we assign a new value to ival through p
cout << *p;       // prints 0
```

A null pinter does not point to any object.

```cpp
int *p1 = nullptr;   // equivalent to int *p1 = 0;
int *p2 = 0;         // directly initializers p2 from the literal constant 0
// must #include cstdlib
int *p2 = NULL;      // equivalent to int *p3 = 0; 
```

It is illegal to assign an **int variable** to a pointer, even if the variable's value happens to be 0.

```cpp
int zero = 0;
pi = zero;     // error ,cannot assign an itn to a pointer

int i = 42;
int *pi2 = &i; 
pi2 = 0;       // pi2 now addresses no object

pi = &ival;    // value in pi is changed; pi now points to ival
*pi = 0;       // value in ival is changed; pi is unchanged
```

### 2.3.3 Understanding Compound Type Declarations

```cpp
int* p1, p2;       // p1 is a pointer to int; p2 is an int

int ival = 1024;
int *pi = &ival;   // pi points to an int
int **ppi = &pi;   // ppi points to a pointer to an int
```

A reference is not an object. Hence, we may not have a pointer to a reference. However, because a pointer is an object, we can define a reference to a pointer:

```cpp
int i = 42;
int *p;        // p is a pointer to int
int *&r = p;   // r is a reference to the pointer p
r = &i;        //  r refers to a pointer; assigning &i to r makes p point to i
*r = 0;        //  dereferencing r yields i, the object to which p points; changes i to 0
```

The easiest way to understand the type of r is to read the definition right to left. The symbol closest to the name of the variable (in this case the & in &r) is the one that has the most immediate effect on the variable’s type. Thus, we know that r is a reference. The rest of the declarator determines the type to which r refers. The next symbol, * in this case, says that the type r refers to is a pointer type. Finally, the base type of the declaration says that r is a reference to a pointer to an int.

It can be easier to understand complicated pointer or reference declarations if you read them from right to left.


## 2.4 _CONST_ Qualifier

when we use an object to initialize another object, it doesn’t matter whether either or both of the objects are consts:

```cpp
int i = 42;
const int ci = i;    // ok: the value in i is copied into ci
int j = ci;          // ok: the value in ci is copied into j
```

### 2.4.1 Reference to _CONST_

Unlike an ordinary reference, a reference to const cannot be used to change the object to which the reference is bound:

```cpp
const int ci = 1024;
const int &r1 = ci;   // ok: both reference and underlying object are const
r1 = 42;              // error: r1 is a reference to const
int &r2 = ci;         // error: non const reference to a const object
```

The first exception is that we can initialize a reference to const from any expression that can be converted to the type of the reference. In particular, we can bind a reference to const to a nonconst object, a literal, or a more general expression:

```cpp
int i = 42;
const int &r1 = i;      // we can bind a const int& to a plain int object
const int &r2 = 42;     // ok: r1 is a reference to const
const int &r3 = r1 * 2; // ok: r3 is a reference to const
int &r4 = r * 2;        // error: r4 is a plain, non const reference
```

```cpp
double dval = 3.14;
const int &ri = dval;
```

the compiler transforms this code into something like

```cpp
const int temp = dval;   // create a temporary const int from the double
const int &ri = temp;    // bind ri to that temporary
```

It is important to realize that a reference to const restricts only what we can do through that reference. Binding a reference to const to an object says nothing about whether the underlying object itself is const. Because the underlying object might be nonconst, it might be changed by other means:

```cpp
int i = 42;
int &r1 = i;          // r1 bound to i
const int &r2 = i;    // r2 also bound to i; but cannot be used to change i
r1 = 0;               // r1 is not const; i is now 0
r2 = 0;               // error: r2 is a reference to const
```

### 2.4.2 Pointers and _CONST_

Must use const pointer to store address of const object. But a const pointer can also store address of non-const object. Like a reference to const, a pointer to const says nothing about whether the object to which the pointer points is const

```cpp
const double pi = 3.14;   // pi is const; its value may not be changed
double *ptr = &pi;        // error: ptr is a plain pointer
const double *cptr = &pi; // ok: cptr may point to a double that is const
*cptr = 42;               // error: cannot assign to *cptr
```


A const pointer must be initialized, and once initialized, its value may not be changed. We indicate that the pointer is const by putting the const after the *. This placement indicates that it is the pointer, not the pointed-to type, that is const:

```cpp
int errNumb = 0;
int *const curErr = &errNumb;  // curErr will always point to errNumb
const double pi = 3.14159;
const double *const pip = &pi; // pip is a const pointer to a const object
```

### 2.4.3 TOP-LEVEL _CONST_

We use the term top-level const to indicate that the pointer itself is a const. When a pointer can point to a const object, we refer to that const as a low-level const.

```cpp
int &r = 0;             // illegal, r must refer to an object.
const int &r = 0;       // legal.
const int &const r2;    // illegal, r2 is a reference that cannot be const.

const int *p;           // legal. a pointer to const int.
int *p1, *const p2;     // illegal, p2 must initialize.
const int ic, &r = ic;  // illegal, ic must initialize.
```

The distinction between top-level and low-level matters when we copy an object. When we copy an object, top-level consts are ignored. On the other hand, low-level const is never ignored. When we copy an object, both objects must have the same low-level const qualification or there must be a conversion between the types of the two objects. In general, **we can convert a nonconst to const but not the other way round.**

```cpp
const int v2 = 0;
int v1 = v2;
int *p1 = &v1;
int &r1 = v1;
const int *p2 = &v2, *const p3 = &i, &r2 = v2;

r1 = v2;  // legal, top-level const in v2 is ignored.
p1 = p2;  // illegal, p2 has a low-level const but p1 doesn't.
p2 = p1;  // legal, we can convert int* to const int*.
p1 = p3;  // illegal, p3 has a low-level const but p1 doesn't.
p2 = p3;  // legal, p2 has the same low-level const qualification as p3.
```

### 2.4.4 _constexpr_ and Constant Expressions

A constant expression is an expression whose value cannot change and that can be evaluated at compile time. A literal is a constant expression. A const object that is initialized from a constant expression is also a constant expression. 

```cpp
const int max_files = 20;    // max_files is a constant expression
const int sz = get_size();   // sz is not a constant expression
```

Under the new standard, we can ask the compiler to verify that a variable is a constant expression by declaring the variable in a constexpr declaration. Variables declared as constexpr are implicitly const and must be initialized by constant expressions:

```cpp
constexpr int mf = 20;        // 20 is a constant expression
constexpr int limit = mf + 1; // mf + 1 is a constant expression
constexpr int sz = size();    // ok only if size is a constexpr function
```

It is important to understand that when we define a pointer in a constexpr declaration, the constexpr specifier applies to the pointer, not the type to which the pointer points:

```cpp
const int *p = nullptr;     // p is a pointer to a const int
constexpr int *q = nullptr; // q is a const pointer to int
```


## 2.5 Dealing with types

### 2.5.1 Type Aliases

We can define a type alias in one of two ways. Traditionally, we use a typedef:

```cpp
typedef double wages;   // wages is a synonym for double
typedef wages base, *p; // base is a synonym for double, p for double*
```

The new standard introduced a second way to define a type alias, via an alias declaration:

```cpp
using SI = Sales_item;  // SI is a synonym for Sales_item
```

Declarations that use type aliases that represent compound types and const can yield surprising results. 

```cpp
typedef char *pstring;
const pstring cstr = 0; // cstr is a constant pointer to char
const pstring *ps;      // ps is a pointer to a constant pointer to char
```

The base type in these declarations is const pstring. As usual, a const that appears in the base type modifies the given type. The type of pstring is “pointer to char.” So, const pstring is a constant pointer to char—not a pointer to const char.

### 2.5.2 The _auto_ Type Specifier

Under the new standard, we can let the compiler figure out the type for us by using the auto type specifier. Unlike type specifiers, such as double, that name a specific type, auto tells the compiler to deduce the type from the initializer. By implication, a variable that uses auto as its type specifier must have an initializer:

```cpp
// the type of item is deduced from the type of the result of adding val1 and val2
auto item = val1 + val2; // item initialized to the result of val1 + val2
```

Because a declaration can involve only a single base type, the initializers for all the variables in the declaration must have types that are consistent with each other:

```cpp
auto i = 0, *p = &i;      // ok: i is int and p is a pointer to int
auto sz = 0, pi = 3.14;   // error: inconsistent types for sz and pi
```

First, as we’ve seen, when we use a reference, we are really using the object to which the reference refers. Second, auto ordinarily ignores top-level consts. As usual in initializations, low-level consts, such as when an initializer is a pointer to const, are kept.

```cpp
const int ci = i, &cr = ci;
auto b = ci;  // b is an int (top-level const in ci is dropped)
auto c = cr;  // c is an int (cr is an alias for ci whose const is top-level)
auto d = &i;  // d is an int*(& of an int object is int*)
auto e = &ci; // e is const int*(& of a const object is low-level const)

// If we want the deduced type to have a top-level const, we must say so explicitly:
const auto f = ci; // deduced type of ci is int; f has type const int
```

### 2.5.3 The _decltype_ Type Specifier

Sometimes we want to define a variable with a type that the compiler deduces from an expression but do not want to use that expression to initialize the variable. For such cases, the new standard introduced a second type specifier, decltype , which returns the type of its operand. The compiler analyzes the expression to determine its type but does not evaluate the expression:

```cpp
decltype(f()) sum = x; // sum has whatever type f returns
```

The way decltype handles top-level const and references differs subtly from the way auto does. When the expression to which we apply decltype is a variable, decltype returns the type of that variable, including top-level const and references.

```cpp
const int ci = 0, &cj = ci;
decltype(ci) x = 0; // x has type const int
decltype(cj) y = x; // y has type const int& and is bound to x
decltype(cj) z;     // error: z is a reference and must be initialized
```

Another important difference between decltype and auto is that the deduction done by decltype depends on the form of its given expression. What can be confusing is that enclosing the name of a variable in parentheses affects the type returned by decltype. When we apply decltype to a variable without any parentheses, we get the type of that variable. If we wrap the variable’s name in one or more sets of parentheses, the compiler will evaluate the operand as an expression. A variable is an expression that can be the left-hand side of an assignment. As a result, decltype on such an expression yields a reference:

```cpp
// decltype of a parenthesized variable is always a reference
decltype((i)) d;    // error: d is int& and must be initialized
decltype(i) e;      // ok: e is an (uninitialized) int
```

Remember that decltype((variable)) (note, double parentheses) is always a reference type, but decltype(variable) is a reference type only if variable is a reference.

## 2.6 Defining our own data strucutres

### 2.6.1 

Because our data structure does not support any operations, we’ll name our version Sales\_data to distinguish it from Sales\_item. We’ll define our class as follows:

```cpp
struct Sales_data {
    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
}; // Don't forget the semicolon
```

### 2.6.2 Using the _Sales\_data_ Class

```cpp
struct Sales_data { /* ... */ } accum, trans, *salesptr;
// equivalent, but better way to define these objects
struct Sales_data { /* ... */ };
Sales_data accum, trans, *salesptr;
```

```cpp
double price = 0;   // price per book, used to calculate total revenue
// read the first transactions: ISBN, number of books sold, price per book
std::cin >> data1.bookNo >> data1.units_sold >> price;
// calculate total revenue from price and units_sold
data1.revenue = data1.units_sold * price;
```

### 2.6.3 Writing Our Own Header Files

As we’ve seen, programs that use Sales\_data also need to include the string header in order to use the bookNo member. As a result, programs that use Sales_data will include the string header twice: once directly and once as a side effect of including Sales\_data.h. Because a header might be included more than once, we need to write our headers in a way that is safe even if the header is included multiple times.

The most common technique for making it safe to include a header multiple times relies on the preprocessor. The preprocessor—which C++ inherits from C—is a program that runs before the compiler and changes the source text of our programs. Our programs already rely on one preprocessor facility, #include. When the preprocessor sees a #include, it replaces the #include with the contents of the specified header.

C++ programs also use the preprocessor to define header guards. Header guards rely on preprocessor variables. Preprocessor variables have one of two possible states: defined or not defined. The #define directive takes a name and defines that name as a preprocessor variable. There are two other directives that test whether a given preprocessor variable has or has not been defined: #ifdef is true if the variable has been defined, and #ifndef is true if the variable has not been defined. If the test is true, then everything following the #ifdef or #ifndef is processed up to the matching #endif.

```cpp
#ifndef SALES_DATA_H
#define SALES_DATA_H
#include <string>
struct Sales_data {
    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};
#endif
```

The first time Sales_data.h is included, the #ifndef test will succeed. The preprocessor will process the lines following #ifndef up to the #endif. As a result, the preprocessor variable SALES_DATA_H will be defined and the contents of Sales_data.h will be copied into our program. If we include Sales_data.h later on in the same file, the #ifndef directive will be false. The lines between it and the #endif directive will be ignored.

Preprocessor variables, including names of header guards, must be unique throughout the program. Typically we ensure uniqueness by basing the guard’s name on the name of a class in the header. **To avoid name clashes with other entities in our programs, preprocessor variables usually are written in all uppercase.**