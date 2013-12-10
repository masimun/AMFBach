//
//  AMFIterator.cpp
//  AMFBach
//
//  Created by Max Dekoninck on 10/12/13.
//  Copyright (c) 2013 Daan en Max. All rights reserved.
//

#include "AMFIterator.h"


class const_iterator
{
public:
    typedef const_iterator self_type;
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef int difference_type;
    typedef std::forward_iterator_tag iterator_category;
    const_iterator(pointer ptr) : ptr_(ptr) { }
    self_type operator++() { self_type i = *this; ptr_++; return i; }
    self_type operator++(int junk) { ptr_++; return *this; }
    const reference operator*() { return *ptr_; }
    const pointer operator->() { return ptr_; }
    bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
    bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
private:
    pointer ptr_;
};