# Chapter7 Classes

## 7.1 Defining Abstract Data Types

The **this** parameter is defined for us implicitly. By default, the type of this is a const pointer to the nonconst version of the class type. We cannot bind this to a const object, which means that we cannot call an ordinary member function on a const object. The language resolves this problem by letting us put const after the parameter list of a member function. A const following the parameter list indicates that this is a pointer to const. Member functions that use const in this way are const member functions.

```cpp
std::string isbn() const { return bookNo; }
```

The fact that **this** is a pointer to const means that const member functions cannot change the object on which they are called. Objects that are const, and references or pointers to const objects, may call only const member functions.

Ordinarily, nonmember functions that are part of the interface of a class should be declared in the same header as the class itself.

```cpp
istream &read(istream &is, Sales_data &item)
{
    double price = 0;
    is >> item.bookNo >> item.units_sold >> price;
    item.revenue = price * item.units_sold;
    return is;
}
ostream &print(ostream &os, const Sales_data &item)
{
    os << item.isbn() << " " << item.units_sold << " "
       << item.revenue << " " << item.avg_price();
    return os;
}
```

The IO classes are types that cannot be copied, so we may only pass them by reference. Moreover, reading or writing to a stream changes that stream, so both functions take ordinary references, not references to const.

The compiler generates a default constructor automatically only if a class declares no constructors.

```cpp
struct Sales_data {
    // constructors added
    Sales_data() = default;  // C++ 11
    Sales_data(const std::string &s): bookNo(s) { }
    Sales_data(const std::string &s, unsigned n, double p):
               bookNo(s), units_sold(n), revenue(p*n) { }
    Sales_data(std::istream &);
}
```

Under the new standard, if we want the default behavior, we can ask the compiler to generate the constructor for us by writing = default after the parameter list. Like any other function, if the = default appears inside the class body, the default constructor will be inlined; if it appears on the definition outside the class, the member will not be inlined by default.

The constructor that has a single string parameter uses that string to initialize bookNo but does not explicitly initialize the units_sold and revenue members. When a member is omitted from the constructor initializer list, it is implicitly initialized using the same process as is used by the synthesized default constructor. In this case, those members are initialized by the in-class initializers. Thus, the constructor that takes a string is equivalent to

```cpp
// has the same behavior as the original constructor defined above
Sales_data(const std::string &s):
           bookNo(s), units_sold(0), revenue(0){ }
```

As with any other member function, when we define a constructor outside of the class body, we must specify the class of which the constructor is a member.

```cpp
Sales_data::Sales_data(std::istream &is)
{
    read(is, *this); // read will read a transaction from is into this object
}
```

In addition to defining how objects of the class type are initialized, classes also control what happens when we copy, assign, or destroy objects of the class type.