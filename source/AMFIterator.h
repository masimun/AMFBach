//
//  AMFIterator.h
//  AMFBach
//
//  Created by Max Dekoninck on 10/12/13.
//  Copyright (c) 2013 Daan en Max. All rights reserved.
//

#ifndef __AMFBach__AMFIterator__
#define __AMFBach__AMFIterator__

#include <iostream>

#endif /* defined(__AMFBach__AMFIterator__) */

class const_iterator
{
public:
    typedef const_iterator self_type;
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef int difference_type;
    typedef std::forward_iterator_tag iterator_category;
    const_iterator(pointer ptr) : ptr_(ptr);
    self_type operator++();
    self_type operator++(int junk);
    const reference operator*();
    const pointer operator->();
    bool operator==(const self_type& rhs);
    bool operator!=(const self_type& rhs);
private:
    pointer ptr_;
};