# Chapter 12 Dynamic Memory

 * Global objects are allocated at program start-up and destroyed when the program ends.
 * Local, automatic objects are created and destroyed when the block in which they are defined is entered and exited.
 * Local static objects are allocated before their first use and are destroyed when the program ends.

**Static memory** is used for local static object, class static data members, and for variables defined outside any function.

**Stack memory** is used for non-static objects definied inside functions.

Objects allocated in static or stack memory are automatically created and destroyed by the compiler. Stack objects exist only while the block in which they are defined is executing; static objects are allocated before they are used, and they are destroyed when the program ends.

Dynamically allocated objects have a lifetime that is independent of where they are created; they exist until they are explicitly freed. Smart pointers ensure that the objects to which they point are automatically freed when it is appropriate to do so.

In addition to static or stack memory, every program also has a pool of memory that it can use. This memory is referred to as the free store or heap. Programs use the heap for objects that they dynamically allocate—that is, for objects that the program allocates at run time. The program controls the lifetime of dynamic objects; our code must explicitly destroy such objects when they are no longer needed.

## 12.1 Dynamic Memory and Smart Pointers

Dynamic memory is managed by **new** and **delete**. In c++ 11, two smart pointer types are provided to manager dynamic objects.

 * **shared_ptr** allows multiple pointers to refer to the same objects.
 * **unique_ptr** owns the object to which it points.
 * **weak_ptr** a weak reference to an object managed by a shared\_ptr.

All three are definied in the **memory** header.

### 12.1.1 The **shared_ptr** Class

Smart pointers are templates. A default initialized smart pointer hold a null pointer.

```cpp
shared_ptr<string> p1;    // shared_ptr that can point at a string
shared_ptr<list<int>> p2; // shared_ptr that can point at a list of ints

// if p1 is not null, check whether it's the empty string
if (p1 && p1->empty())
    *p1 = "hi";  // if so, dereference p1 to assign a new value to that string
```

The safest way to allocate and use dynamic memory is to call a library function named make\_shared. This function allocates and initializes an object in dynamic memory and returns a shared\_ptr that points to that object.

```cpp
// shared_ptr that points to an int with value 42
shared_ptr<int> p3 = make_shared<int>(42);
// p4 points to a string with value 9999999999
shared_ptr<string> p4 = make_shared<string>(10, '9');
// p5 points to an int that is value initialized to 0
shared_ptr<int> p5 = make_shared<int>();
```

We can use auto to make it easier to define an object hold the result of make_shared:

```cpp
// p6 points to a dynamically allocated, empty vector<string>
auto p6 = make_shared<vector<string>>();
```