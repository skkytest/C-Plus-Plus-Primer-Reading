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

We can think of a **shared_ptr** as if it has an associated counter, usually refered to as reference count. It incresease when:

 * we use it to initalize another shared_ptr
 * use it as the right-hand operand
 * pass it to or return it from a function by value

It decreases when:

 * assign a new value to the **shared_ptr**
 * local **shared_ptr** goes out of scope

```cpp
auto r = make_shared<int>(42); // int to which r points has one user
r = q;  // assign to r, making it point to a different address
        // increase the use count for the object to which q points
        // reduce the use count of the object to which r had pointed
        // the object r had pointed to has no users; that object is automatically freed
```

When the last shared\_ptr pointing to an object is destroyed, the shared\_ptr class automatically destroys the object to which that shared\_ptr points. It does so through another special member function known as a destructor. The fact that the shared\_ptr class automatically frees dynamic objects when they are no longer needed makes it fairly easy to use dynamic memory.

```cpp
// factory returns a shared_ptr pointing to a dynamically allocated object
shared_ptr<Foo> factory(T arg)
{
    // process arg as appropriate
    // shared_ptr will take care of deleting this memory
    return make_shared<Foo>(arg);
}
//Because factory returns a shared_ptr, we can be sure that the object allocated by factory will be freed when appropriate.

void use_factory(T arg)
{
    shared_ptr<Foo> p = factory(arg);
    // use p
} // p goes out of scope; the memory to which p points is automatically freed
```

If you put shared_ptrs in a container, and you subsequently need to use some, but not all, of the elements, remember to erase the elements you no longer need.

One common reason to use dynamic memory is to allow multiple objects to share the same state. **For an example class, check file 12_02.h**

### 12.1.2 Managing Memory Directly

The language itself defines two operators that allocate and free dynamic memory. The new operator allocates memory, and delete frees memory allocated by new. However, using these operators to manage memory is considerably more error-prone than using a smart pointer. 

```cpp
string *ps1 = new string;  // default initialized to the empty string
string *ps = new string(); // value initialized to the empty string
int *pi1 = new int;        // default initialized; *pi1 is undefined
int *pi2 = new int();      // value initialized to 0; *pi2 is 0

int *pi = new int(1024); // object to which pi points has value 1024
string *ps = new string(10, '9');   // *ps is "9999999999"
// vector with ten elements with values from 0 to 9
vector<int> *pv = new vector<int>{0,1,2,3,4,5,6,7,8,9}; // Only for c++ 11 and later

// we can use auto only with a single initializer inside parentheses:
auto p1 = new auto(obj);   // p points to an object of the type of obj
                           // that object is initialized from obj
auto p2 = new auto{a,b,c}; // error: must use parentheses for the initializer

// allocate and initialize a const int
const int *pci = new const int(1024);
// allocate a default-initialized const empty string
const string *pcs = new const string;

// if allocation fails, new returns a null pointer
int *p1 = new int; // if allocation fails, new throws std::bad_alloc
int *p2 = new (nothrow) int; // if allocation fails, new returns a null pointer
```

Deleting a pointer to memory that was not allocated by new, or deleting the same pointer value more than once, is undefined:

```cpp
int i, *pi1 = &i, *pi2 = nullptr;
double *pd = new double(33), *pd2 = pd;
delete i;   // error: i is not a pointer
delete pi1; // undefined: pi1 refers to a local
delete pd;  // ok
delete pd2; // undefined: the memory pointed to by pd2 was already freed
delete pi2; // ok: it is always ok to delete a null pointer

const int *pci = new const int(1024);
delete pci;  // ok: deletes a const object
```

Different from shared\_ptr, a dynamic object managed through a built-in pointer exists until it is explicitly deleted.

After the delete, the pointer becomes what is referred to as a dangling pointer. A dangling pointer is one that refers to memory that once held an object but no longer does so.

**Check 12.06.h for comparison between shared\_ptr and new/delete.**