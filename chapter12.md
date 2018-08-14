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

`p.use_count()` returns the number of objects sharing with p, may be a slow operation, intended primarily for dubugging purpose.

`p.unique` returns true is `p.use_count()` is one; false otherwise.

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

### 12.1.3 Using *shared_ptr*s with *new*

We can also initialize a smart pointer from a pointer returned by new, but we must use the direct form of initializationto initialize a smart pointer. We cannot convert a built-in pointer to a smart pointer.

```cpp
shared_ptr<int> p1 = new int(1024);  // error: must use direct initialization
shared_ptr<int> p2(new int(1024));   // ok: uses direct initialization

auto p = new int();
// ptr is created and initialized when process is called
void process(shared_ptr<int> ptr)
{
    // use ptr
} // ptr goes out of scope and is destroyed
process(p); // error: plain pointer can not convert to smart pointer implicitly.

shared_ptr<int> clone(int p) {
    return new int(p); // error: implicit conversion to shared_ptr<int>

    // ok: explicitly create a shared_ptr<int> from int*
    return shared_ptr<int>(new int(p));
}
```

:exclamation: Don’t Mix Ordinary Pointers and Smart Pointers.

```cpp
shared_ptr<int> p(new int(42)); // reference count is 1
process(p); // copying p increments its count; in process the reference count is 2
int i = *p; // ok: reference count is 1

int *x(new int(1024)); // dangerous: x is a plain pointer, not a smart pointer
process(x);  // error: cannot convert int* to shared_ptr<int>
process(shared_ptr<int>(x)); // legal, but the memory will be deleted!
int j = *x;  // undefined: x is a dangling pointer!
```

```cpp
int main()
{
    std::shared_ptr<int> p(new int(42));
    /**
     * @brief   std::shared_ptr<int>(p.get()) construct a temporary shared_ptr and copy it
     *          to the parameter.However it is not a copy of p. As a result, at end of this
     *          main function p will free the memory that has been freed inside process ().
     *          That's why "double freed or corruption" was generated.
     */
    process(std::shared_ptr<int>(p.get()));
    return 0;
}
```

:exclamation: Don’t Use get to Initialize or Assign Another Smart Pointer.

The smart pointer types define a function named get that returns a built-in pointer to the object that the smart pointer is managing. The code that uses the return from get must not delete that pointer.

```cpp
shared_ptr<int> p(new int(42)); // reference count is 1
int *q = p.get();  // ok: but don't use q in any way that might delete its pointer
{ // new block
// undefined: two independent shared_ptrs point to the same memory
shared_ptr<int>(q);
} // block ends, q is destroyed, and the memory to which q points is freed
int foo = *p; // undefined; the memory to which p points was freed
```

We can use reset to assign a new pointer to a shared_ptr.

```cpp
p = new int(1024);       // error: cannot assign a pointer to a shared_ptr
p.reset(new int(1024));  // ok: p points to a new object

if (!p.unique())
    p.reset(new string(*p)); // we aren't alone; allocate a new copy
*p += newVal; // now that we know we're the only pointer, okay to change this object
```

### 12.1.4 Smart Pointes and Exceptions

When we use a smart pointer, the smart pointer class ensures that memory is freed when it is no longer needed even if the block is exited prematurely. In contrast, memory that we manage directly is not automatically freed when an exception occurs. 

```cpp
void f()
{
    shared_ptr<int> sp(new int(42)); // allocate a new object
   // code that throws an exception that is not caught inside f
} // shared_ptr freed automatically when the function ends
```

Classes that allocate resources—and that do not define destructors to free those resources—can be subject to the same kind of errors that arise when we use dynamic memory.

```cpp
struct destination;  // represents what we are connecting to
struct connection;   // information needed to use the connection
connection connect(destination*);  // open the connection
void disconnect(connection);       // close the given connection
void f(destination &d /* other parameters */)
{
    // get a connection; must remember to close it when done
    connection c = connect(&d);
    // use the connection
    // if we forget to call disconnect before exiting f, there will be no way to close c
}
```

To use a shared\_ptr to manage a connection, we must first define a function to use in place of delete. It must be possible to call this deleter function with the pointer stored inside the shared\_ptr. In this case, our deleter must take a single argument of type connection*:

```cpp
void end_connection(connection *p) { disconnect(*p); }

void f(destination &d /* other parameters */)
{
    connection c = connect(&d);
    shared_ptr<connection> p(&c, end_connection);
    // use the connection
    // when f exits, even if by an exception, the connection will be properly closed
}
```

When p is destroyed, it won’t execute delete on its stored pointer. Instead, p will call end\_connection on that pointer. In turn, end\_connection will call disconnect, thus ensuring that the connection is closed. If f exits normally, then p will be destroyed as part of the return. Moreover, p will also be destroyed, and the connection will be closed, if an exception occurs.

 * Don’t use the same built-in pointer value to initialize (or reset) more than one smart pointer.
 * Don’t delete the pointer returned from get().
 * Don’t use get() to initialize or reset another smart pointer.
 * If you use a smart pointer to manage a resource other than memory allocated by new, remember to pass a deleter 

### 12.1.5 *unique_ptr*

A unique\_ptr “owns” the object to which it points. Unlike shared\_ptr, only one unique\_ptr at a time can point to a given object. The object to which a unique\_ptr points is destroyed when the unique\_ptr is destroyed.

Unlike shared\_ptr, there is no library function comparable to make\_shared that returns a unique\_ptr. Instead, when we define a unique\_ptr, we bind it to a pointer returned by new. As with shared\_ptrs, we must use the direct form of initialization.

Because a unique\_ptr owns the object to which it points, unique\_ptr does not support ordinary copy or assignment. But we can transfer ownership from one (nonconst) unique\_ptr to another by calling release or reset:

```cpp
unique_ptr<double> p1;  // unique_ptr that can point at a double
unique_ptr<int> p2(new int(42)); // p2 points to int with value 42

unique_ptr<string> p1(new string("Stegosaurus"));
unique_ptr<string> p2(p1);  // error: no copy for unique_ptr
unique_ptr<string> p3;
p3 = p2;                    // error: no assign for unique_ptr

// transfers ownership from p1 (which points to the string Stegosaurus) to p2
unique_ptr<string> p2(p1.release()); // release makes p1 null
unique_ptr<string> p3(new string("Trex"));
// transfers ownership from p3 to p2
p2.reset(p3.release()); // reset deletes the memory to which p2 had pointed
```

```cpp
int ix = 1024, *pi = &ix, *pi2 = new int(2048);
typedef std::unique_ptr<int> IntP;

IntP p0(ix);  // error: invalid conversion from 'int' to 'std::unique_ptr<int>::pointer
IntP p1(pi);  // The code below can compile, but will cause error at run time. The object is not allocate using new.
IntP p2(pi2); // This code can compile, but cause a dangling pointer at run time. The unique_ptr will free the object the raw pointer is pointing to.
IntP p3(&ix); // When the unique_ptr goes out of scope, it will call delete to free an obeject not allocated using new.
IntP p4(new int(2048));  // Recommended
IntP p5(p2.get());  // error: double free
```

Calling release breaks the connection between a unique\_ptr and the object it had been managing. Often the pointer returned by release is used to initialize or assign another smart pointer. However, if we do not use another smart pointer to hold the pointer returned from release, our program takes over responsibility for freeing that resource:

```cpp
p2.release(); // WRONG: p2 won't free the memory and we've lost the pointer
auto p = p2.release(); // ok, but we must remember to delete(p)
```

There is one exception to the rule that we cannot copy a unique\_ptr: We can copy or assign a unique\_ptr that is about to be destroyed. The most common example is when we return a unique\_ptr from a function:

```cpp
unique_ptr<int> clone(int p) {
    // ok: explicitly create a unique_ptr<int> from int*
    return unique_ptr<int>(new int(p));
}

unique_ptr<int> clone(int p) {
    unique_ptr<int> ret(new int (p));
    // . . .
    return ret;
}
```

Like shared\_ptr, by default, unique\_ptr uses delete to free the object to which a unique\_ptr points. As with shared\_ptr, we can override the default deleter in a unique\_ptr. However, the way unique\_ptr manages its deleter is differs from the way shared\_ptr does. We must supply the deleter type inside the angle brackets along with the type to which the unique\_ptr can point. We supply a callable object of the specified type when we create or reset an object of this type:

```cpp
// p points to an object of type objT and uses an object of type delT to free that object
// it will call an object named fcn of type delT
unique_ptr<objT, delT> p (new objT, fcn);

void f(destination &d /* other needed parameters */)
{
    connection c = connect(&d);  // open the connection
    // when p is destroyed, the connection will be closed
    unique_ptr<connection, decltype(end_connection)*>
        p(&c, end_connection);
    // use the connection
    // when f exits, even if by an exception, the connection will be properly closed
}
```