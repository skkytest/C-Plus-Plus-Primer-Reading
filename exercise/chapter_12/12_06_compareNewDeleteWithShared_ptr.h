// From github https://github.com/Mooophy/Cpp-Primer/blob/master/ch12/ex12_06.cpp
// Exercise 12.6:
// Write a function that returns a dynamically allocated vector of ints.
// Pass that vector to another function that reads the standard input to
// give values to the elements. Pass the vector to another function to print
// the values that were read.
// Remember to delete the vector at the appropriate time.
//

#include <iostream>
#include <vector>

using Ptr = std::vector<int>*;

auto make_dynamically()
{
    return new std::vector < int > { };
}

auto populate(Ptr vec)
{
    for (int i; std::cout << "Pls Enter:\n", std::cin >> i; vec->push_back(i));
    return vec;
}

auto print(Ptr vec) -> std::ostream&
{
    for (auto i : *vec) std::cout << i << " ";
    return std::cout;
}

int main()
{
    auto vec = populate(make_dynamically());
    print(vec) << std::endl;
    delete vec; // Delete is required

    return 0;
}

// From github https://github.com/Mooophy/Cpp-Primer/blob/master/ch12/ex12_07.cpp
// Exercise 12.7:
// Redo the previous exercise, this time using shared_ptr.
//

#include <iostream>
#include <vector>
#include <memory>

using Sptr = std::shared_ptr < std::vector<int> > ;

auto make_with_shared_ptr()
{
    return std::make_shared<std::vector<int>>();
}

auto populate(Sptr vec)
{
    for (int i; std::cout << "Pls Enter:\n", std::cin >> i; vec->push_back(i));
    return vec;
}

auto print(Sptr vec) -> std::ostream&
{
    for (auto i : *vec) std::cout << i << " ";
    return std::cout;
}

int main()
{
    auto vec = populate(make_with_shared_ptr());
    print(vec) << std::endl;
    // No need to delete
    return 0;
}