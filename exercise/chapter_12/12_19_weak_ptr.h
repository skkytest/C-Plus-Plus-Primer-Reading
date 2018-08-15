//  From github https://github.com/Mooophy/Cpp-Primer/blob/master/ch12/ex12_19.h
//  ex12_19.h
//  Exercise 12.19
//
//  Created by pezy on 12/26/14.
//
//  Define your own version of StrBlobPtr and
//  update your StrBlob class with the appropriate friend declaration and begin and end members.
//
//  @See    ex12_02.h

#ifndef CP5_ex12_19_h
#define CP5_ex12_19_h

#include <vector>
#include <string>
#include <initializer_list>
#include <memory>
#include <stdexcept>
using std::vector; using std::string;

class StrBlobPtr;

class StrBlob {
public:
    using size_type = vector<string>::size_type;
    friend class StrBlobPtr;

    StrBlobPtr begin(); // return StrBlobPtr to the first element
    StrBlobPtr end();   // and one past the last element

    StrBlob():data(std::make_shared<vector<string>>()) { }
    StrBlob(std::initializer_list<string> il):data(std::make_shared<vector<string>>(il)) { }

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }

    void push_back(const string &t) { data->push_back(t); }
    void pop_back() {
        check(0, "pop_back on empty StrBlob");
        data->pop_back();
    }

    std::string& front() {
        check(0, "front on empty StrBlob");
        return data->front();
    }

    std::string& back() {
        check(0, "back on empty StrBlob");
        return data->back();
    }

    const std::string& front() const {
        check(0, "front on empty StrBlob");
        return data->front();
    }
    const std::string& back() const {
        check(0, "back on empty StrBlob");
        return data->back();
    }

private:
    void check(size_type i, const string &msg) const {
        if (i >= data->size()) throw std::out_of_range(msg);
    }

private:
    std::shared_ptr<vector<string>> data;
};

// throws an exception on attempts to access a nonexistent element
class StrBlobPtr {
public:
    StrBlobPtr():curr(0) { }
    StrBlobPtr(StrBlob &a, size_t sz = 0):wptr(a.data), curr(sz) { }
    bool operator!=(const StrBlobPtr& p) { return p.curr != curr; }
    string& deref() const {
        auto p = check(curr, "dereference past end");
        return (*p)[curr]; // (*p) is the vector to which this object points
    }
    StrBlobPtr& incr() {
        check(curr, "increment past end of StrBlobPtr");
        ++curr;
        return *this;
    }

private:
    // check returns a shared_ptr to the vector if the check succeeds
    std::shared_ptr<vector<string>> check(size_t i, const string &msg) const {
        auto ret = wptr.lock(); // is the vector still around?
        if (!ret) throw std::runtime_error("unbound StrBlobPtr");
        if (i >= ret->size()) throw std::out_of_range(msg);
        return ret; // otherwise, return a shared_ptr to the vector
    }
    // store a weak_ptr, which means the underlying vector might be destroyed
    std::weak_ptr<vector<string>> wptr;
    size_t curr;
};

#endif



// In .cpp file
#include "ex12_19.h"

// these members can’t be defined until StrStrBlob and StrStrBlobPtr are defined
StrBlobPtr StrBlob::begin()
{
    return StrBlobPtr(*this);
}
StrBlobPtr StrBlob::end()
{
    return StrBlobPtr(*this, data->size());
}

// an example of using class
#include "ex12_19.h"
#include <fstream>
#include <iostream>

int main()
{
    std::ifstream ifs("../data/book.txt");
    StrBlob blob;
    for (std::string str; std::getline(ifs, str); )
        blob.push_back(str);
    for (StrBlobPtr pbeg(blob.begin()), pend(blob.end()); pbeg != pend; pbeg.incr())
        std::cout << pbeg.deref() << std::endl;
}