///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_INDIRECT_VECTOR_INCLUDED
#define ETL_INDIRECT_VECTOR_INCLUDED

#include "platform.h"
#include "vector.h"
#include "pool.h"

#include "stl/iterator.h"
#include "stl/functional.h"

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_NO_STL)
  #include <initializer_list>
#endif

#ifdef ETL_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#undef ETL_FILE
#define ETL_FILE "53"

//*****************************************************************************
///\defgroup indirect_vector indirect_vector
/// A indirect_vector with the capacity defined at compile time. Objects are allocated from a pool and stored as pointers.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup vector
  /// Vector full exception.
  //***************************************************************************
  class indirect_vector_buffer_missmatch : public vector_exception
  {
  public:

    indirect_vector_buffer_missmatch(string_type file_name_, numeric_type line_number_)
      : vector_exception(ETL_ERROR_TEXT("indirect_vector:buffer_missmatch", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for specifically sized vectors.
  /// Can be used as a reference type for all vectors containing a specific type.
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  class iindirect_vector
  {
  public:

    typedef T                                     value_type;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
#if ETL_CPP11_SUPPORTED
    typedef T&&                                   rvalue_reference;
#endif
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef pointer const *                       indirect_pointer;
    typedef const_pointer const *                 indirect_const_pointer;

    typedef typename etl::ivector<T*>::size_type       size_type;
    typedef typename etl::ivector<T*>::difference_type difference_type;

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public std::iterator<std::random_access_iterator_tag, T>
    {
    public:

      friend class iindirect_vector;

      iterator()
      {
      }

      iterator(const iterator& other)
        : lookup_itr(other.lookup_itr)
      {
      }

      iterator& operator ++()
      {
        ++lookup_itr;
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        ++lookup_itr;
        return temp;
      }

      iterator& operator --()
      {
        --lookup_itr;
        return *this;
      }

      iterator operator --(int)
      {
        iterator temp(*this);
        --lookup_itr;
        return temp;
      }

      iterator operator =(const iterator& other)
      {
        lookup_itr = other.lookup_itr;
        return *this;
      }

      iterator operator +=(size_type n)
      {
        lookup_itr += n;
        return *this;
      }

      iterator operator -=(size_type n)
      {
        lookup_itr -= n;
        return *this;
      }

      reference operator *()
      {
        return **lookup_itr;
      }

      const_reference operator *() const
      {
        return **lookup_itr;
      }

      pointer operator &()
      {
        return &(**lookup_itr);
      }

      const_pointer operator &() const
      {
        return &(**lookup_itr);
      }

      pointer operator ->()
      {
        return &(**lookup_itr);
      }

      const_pointer operator ->() const
      {
        return &(**lookup_itr);
      }

      friend iterator operator +(const iterator& lhs, difference_type offset)
      {
        iterator result(lhs);
        result += offset;
        return result;
      }

      friend iterator operator -(const iterator& lhs, difference_type offset)
      {
        iterator result(lhs);
        result -= offset;
        return result;
      }

      friend difference_type operator -(const iterator& lhs, const iterator& rhs)
      {
        return lhs.lookup_itr - rhs.lookup_itr;
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.lookup_itr == rhs.lookup_itr;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

      friend bool operator < (const iterator& lhs, const iterator& rhs)
      {
        return lhs.lookup_itr < rhs.lookup_itr;
      }

    private:

      typedef typename etl::ivector<T*>::iterator lookup_itr_t;

      iterator(lookup_itr_t itr_)
        : lookup_itr(itr_)
      {
      }

      lookup_itr_t lookup_itr;
    };

    //*************************************************************************
    /// const_iterator.
    //*************************************************************************
    class const_iterator : public std::iterator<std::random_access_iterator_tag, const T>
    {
    public:

      friend class iindirect_vector;

      const_iterator()
      {
      }

      const_iterator(const const_iterator& other)
        : lookup_itr(other.lookup_itr)
      {
      }

      const_iterator(const typename iindirect_vector::iterator& other)
        : lookup_itr(other.lookup_itr)
      {
      }

      const_iterator& operator ++()
      {
        ++lookup_itr;
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        ++lookup_itr;
        return temp;
      }

      const_iterator& operator --()
      {
        --lookup_itr;
        return *this;
      }

      const_iterator operator --(int)
      {
        const_iterator temp(*this);
        --lookup_itr;
        return temp;
      }

      const_iterator operator +=(size_type n)
      {
        lookup_itr += n;
        return *this;
      }

      const_iterator operator -=(size_type n)
      {
        lookup_itr -= n;
        return *this;
      }

      const_iterator operator =(const const_iterator& other)
      {
        lookup_itr = other.lookup_itr;
        return *this;
      }

      const_reference operator *() const
      {
        return **lookup_itr;
      }

      const_pointer operator &() const
      {
        return &(**lookup_itr);
      }

      const_pointer operator ->() const
      {
        return &(**lookup_itr);
      }

      friend const_iterator operator +(const const_iterator& lhs, difference_type offset)
      {
        const_iterator result(lhs);
        result += offset;
        return result;
      }

      friend const_iterator operator -(const const_iterator& lhs, difference_type offset)
      {
        const_iterator result(lhs);
        result -= offset;
        return result;
      }

      friend difference_type operator -(const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.lookup_itr - rhs.lookup_itr;
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.lookup_itr == rhs.lookup_itr;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

      friend bool operator < (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.lookup_itr < rhs.lookup_itr;
      }

    private:

      typedef typename etl::ivector<T*>::const_iterator lookup_itr_t;

      const_iterator(lookup_itr_t itr_)
        : lookup_itr(itr_)
      {
      }

      lookup_itr_t lookup_itr;
    };

    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  protected:

    typedef typename etl::parameter_type<T>::type parameter_t;

  public:

    //*********************************************************************
    /// Returns an iterator to the beginning of the indirect_vector.
    ///\return An iterator to the beginning of the indirect_vector.
    //*********************************************************************
    iterator begin()
    {
      return iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the indirect_vector.
    ///\return A const iterator to the beginning of the indirect_vector.
    //*********************************************************************
    const_iterator begin() const
    {
      return const_iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns an iterator to the end of the indirect_vector.
    ///\return An iterator to the end of the indirect_vector.
    //*********************************************************************
    iterator end()
    {
      return iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the indirect_vector.
    ///\return A const iterator to the end of the indirect_vector.
    //*********************************************************************
    const_iterator end() const
    {
      return const_iterator(lookup.end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the indirect_vector.
    ///\return A const iterator to the beginning of the indirect_vector.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(lookup.begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the indirect_vector.
    ///\return A const iterator to the end of the indirect_vector.
    //*********************************************************************
    const_iterator cend() const
    {
      return const_iterator(lookup.cend());
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the indirect_vector.
    ///\return Iterator to the reverse beginning of the indirect_vector.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(end());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the indirect_vector.
    ///\return Const iterator to the reverse beginning of the indirect_vector.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(end());
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the indirect_vector.
    ///\return Reverse iterator to the end + 1 of the indirect_vector.
    //*********************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(begin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the indirect_vector.
    ///\return Const reverse iterator to the end + 1 of the indirect_vector.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(begin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the indirect_vector.
    ///\return Const reverse iterator to the reverse beginning of the indirect_vector.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(cend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the indirect_vector.
    ///\return Const reverse iterator to the end + 1 of the indirect_vector.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(cbegin());
    }

    //*********************************************************************
    /// Resizes the indirect_vector.
    /// If asserts or exceptions are enabled and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    //*********************************************************************
    void resize(size_t new_size)
    {
      resize(new_size, T());
    }

    //*********************************************************************
    /// Resizes the indirect_vector.
    /// If asserts or exceptions are enabled and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    ///\param value   The value to fill new elements with. Default = default constructed value.
    //*********************************************************************
    void resize(size_t new_size, T value)
    {
      ETL_ASSERT(new_size <= capacity(), ETL_ERROR(vector_full));

      if (new_size <= capacity())
      {
        if (new_size > size())
        {
          size_type n = new_size - size();

          while (n-- != 0U)
          {
            T* p = storage.create<T>(value);
            lookup.push_back(p);
          }
        }
        else
        {
          size_type n = size() - new_size;

          while (n-- != 0U)
          {
            pop_back();
          }
        }
      }
    }

    //*********************************************************************
    /// Does nothing.
    //*********************************************************************
    void reserve(size_t)
    {
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference operator [](size_t i)
    {
      return *lookup[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference operator [](size_t i) const
    {
      return *lookup[i];
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::vector_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference at(size_t i)
    {
      return *lookup.at(i);
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::vector_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference at(size_t i) const
    {
      return *lookup.at(i);
    }

    //*********************************************************************
    /// Returns a reference to the first element.
    ///\return A reference to the first element.
    //*********************************************************************
    reference front()
    {
      return *(lookup.front());
    }

    //*********************************************************************
    /// Returns a const reference to the first element.
    ///\return A const reference to the first element.
    //*********************************************************************
    const_reference front() const
    {
      return *(lookup.front());
    }

    //*********************************************************************
    /// Returns a reference to the last element.
    ///\return A reference to the last element.
    //*********************************************************************
    reference back()
    {
      return *(lookup.back());
    }

    //*********************************************************************
    /// Returns a const reference to the last element.
    ///\return A const reference to the last element.
    //*********************************************************************
    const_reference back() const
    {
      return *(lookup.back());
    }

    //*********************************************************************
    /// Returns a pointer to the beginning of the internal lookup vector data.
    /// These are a list of pointers to objects
    ///\return An indirect pointer to the beginning of the internal lookup vector data.
    //*********************************************************************
    indirect_pointer data()
    {
      return lookup.data();
    }

    //*********************************************************************
    /// Returns a pointer to the beginning of the internal lookup vector data.
    /// These are a list of pointers to objects
    ///\return An indirect pointer to the beginning of the internal lookup vector data.
    //*********************************************************************
    indirect_const_pointer data() const
    {
      return lookup.data();
    }

    //*********************************************************************
    /// Assigns values to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector does not have enough free space.
    /// If asserts or exceptions are enabled, emits vector_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
      ETL_STATIC_ASSERT((etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<typename std::iterator_traits<TIterator>::value_type>::type>::value), "Iterator type does not match container type");

#if defined(ETL_DEBUG)
      difference_type d = std::distance(first, last);
      ETL_ASSERT(static_cast<size_t>(d) <= capacity(), ETL_ERROR(vector_full));
#endif

      initialise();

      while (first != last)
      {
        T* p = storage.create<T>(*first);
        lookup.push_back(p);
        ++first;
      }
    }

    //*********************************************************************
    /// Assigns values to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector does not have enough free space.
    ///\param n     The number of elements to add.
    ///\param value The value to insert for each element.
    //*********************************************************************
    void assign(size_t n, parameter_t value)
    {
      ETL_ASSERT(n <= capacity(), ETL_ERROR(vector_full));

      initialise();

      while (n-- != 0U)
      {
        T* p = storage.create<T>(value);
        lookup.push_back(p);
      }
    }

    //*************************************************************************
    /// Clears the indirect_vector.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*********************************************************************
    /// Inserts a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(const_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != capacity(), ETL_ERROR(vector_full));
#endif
      T* p = storage.create<T>(value);
      lookup.push_back(p);
    }

#if ETL_CPP11_SUPPORTED
    //*********************************************************************
    /// Inserts a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(rvalue_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != capacity(), ETL_ERROR(vector_full));
#endif
      T* p = storage.create<T>(std::move(value));
      lookup.push_back(p);
    }
#endif

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_VECTOR_FORCE_CPP03)
    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename ... Args>
    void emplace_back(Args && ... args)
    {
      T* p = storage.create<T>(std::forward<Args>(args)...);
      lookup.push_back(p);
    }
#else
    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1>
    void emplace_back(const T1& value1)
    {
      T* p = storage.create<T>(T(value1));
      lookup.push_back(p);
    }

    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1, typename T2>
    void emplace_back(const T1& value1, const T2& value2)
    {
      T* p = storage.create<T>(T(value1, value2));
      lookup.push_back(p);
    }

    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1, typename T2, typename T3>
    void emplace_back(const T1& value1, const T2& value2, const T3& value3)
    {
      T* p = storage.create<T>(T(value1, value2, value3));
      lookup.push_back(p);
    }

    //*********************************************************************
    /// Constructs a value at the end of the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    void emplace_back(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      T* p = storage.create<T>(T(value1, value2, value3, value4));
      lookup.push_back(p);
    }
#endif

    //*************************************************************************
    /// Removes an element from the end of the indirect_vector.
    //*************************************************************************
    void pop_back()
    {
      ETL_ASSERT(!empty(), ETL_ERROR(vector_empty));

      reference object = back();
      storage.destroy<T>(etl::addressof(object));
      lookup.pop_back();
    }

    //*********************************************************************
    /// Inserts a value to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param position The position to insert before.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, const_reference value)
    {
      ETL_ASSERT(size() + 1 <= capacity(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(value));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************************************************
    /// Inserts a value to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector is already full.
    ///\param position The position to insert before.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, rvalue_reference value)
    {
      ETL_ASSERT(size() + 1 <= capacity(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(std::move(value)));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }
#endif

    //*************************************************************************
    /// Emplaces a value to the vector at the specified position.
    //*************************************************************************
#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT)
    template <typename ... Args>
    iterator emplace(iterator position, Args && ... args)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(std::forward<Args>(args)...));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }
#else
    template <typename T1>
    iterator emplace(iterator position, const T1& value1)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(value1));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }

    template <typename T1, typename T2>
    iterator emplace(iterator position, const T1& value1, const T2& value2)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(value1, value2));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }

    template <typename T1, typename T2, typename T3>
    iterator emplace(iterator position, const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(value1, value2, value3));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }

    template <typename T1, typename T2, typename T3, typename T4>
    iterator emplace(iterator position, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      T* p = storage.create<T>(T(value1, value2, value3, value4));
      position = iterator(lookup.insert(position.lookup_itr, p));

      return position;
    }
#endif

    //*********************************************************************
    /// Inserts 'n' values to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector does not have enough free space.
    ///\param position The position to insert before.
    ///\param n        The number of elements to add.
    ///\param value    The value to insert.
    //*********************************************************************
    void insert(iterator position, size_t n, parameter_t value)
    {
      ETL_ASSERT((size() + n) <= capacity(), ETL_ERROR(vector_full));

      // Make space for the new lookup pointers.
      typename etl::ivector<T*>::iterator lookup_itr = position.lookup_itr;
      lookup.insert(lookup_itr, n, nullptr);

      while (n-- != 0U)
      {
        T* p = storage.create<T>(value);
        *lookup_itr++ = p;
      }
    }

    //*********************************************************************
    /// Inserts a range of values to the indirect_vector.
    /// If asserts or exceptions are enabled, emits vector_full if the indirect_vector does not have enough free space.
    ///\param position The position to insert before.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    void insert(iterator position, TIterator first, TIterator last)
    {
      size_t count = size_t(std::distance(first, last));

      ETL_ASSERT((size() + count) <= capacity(), ETL_ERROR(vector_full));

      // Make space for the new lookup pointers.
      typename etl::ivector<T*>::iterator lookup_itr = position.lookup_itr;
      lookup.insert(lookup_itr, count, nullptr);

      while (first != last)
      {
        T* p = storage.create<T>(*first);
        *lookup_itr++ =  p;
        ++first;
      }
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      storage.destroy<T>(etl::addressof(*i_element));

      return iterator(lookup.erase(i_element.lookup_itr));
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the
    /// element pointed by first, but not the one pointed by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator first, iterator last)
    {
      iterator element = first;

      while (element != last)
      {
        storage.destroy<T>(etl::addressof(*element));
        ++element;
      }

      lookup.erase(first.lookup_itr, last.lookup_itr);

      return last;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    iindirect_vector& operator = (const iindirect_vector& rhs)
    {
      if (&rhs != this)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    iindirect_vector& operator = (iindirect_vector&& rhs)
    {
      if (&rhs != this)
      {
        clear();
        iterator itr = rhs.begin();
        while (itr != rhs.end())
        {
          push_back(std::move(*itr));
          ++itr;
        }

        rhs.initialise();
      }

      return *this;
    }
#endif

    //*************************************************************************
    /// Gets the current size of the indirect_vector.
    ///\return The current size of the indirect_vector.
    //*************************************************************************
    size_type size() const
    {
      return lookup.size();
    }

    //*************************************************************************
    /// Gets the current capacity of the indirect_vector.
    ///\return The capacity of the indirect_vector.
//*************************************************************************
    size_type capacity() const
    {
      return lookup.capacity();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the indirect_vector.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return lookup.empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the indirect_vector.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return lookup.full();
    }

    //*************************************************************************
    /// Returns the maximum size.
    ///\return The maximum size.
    //*************************************************************************
    size_type max_size() const
    {
      return lookup.max_size();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_type available() const
    {
      return lookup.available();
    }

    //*************************************************************************
    /// Sorts the indirect vector using the default 'less'.
    //*************************************************************************
    void sort()
    {
      sort_function(begin(), end(), std::less<T>());
    }

    //*************************************************************************
    /// Sorts the indirect vector using a supplied compare function.
    //*************************************************************************
    template <typename TCompare>
    void sort(TCompare compare)
    {
      sort_function(begin(), end(), compare);
    }

    //*************************************************************************
    /// Sorts the indirect vector between two iterators.
    //*************************************************************************
    void sort(iterator first, iterator last)
    {
      sort_function(first, last, std::less<T>());
    }

    //*************************************************************************
    /// Sorts the indirect vector between two iterators.
    //*************************************************************************
    template <typename TCompare>
    void sort(iterator first, iterator last, TCompare compare)
    {
      sort_function(first, last, compare);
    }

    //*************************************************************************
    /// Sorts the indirect vector using the default 'less'.
    //*************************************************************************
    void stable_sort()
    {
      stable_sort_function(begin(), end(), std::less<T>());
    }

    //*************************************************************************
    /// Sorts the indirect vector using a supplied compare function.
    //*************************************************************************
    template <typename TCompare>
    void stable_sort(TCompare compare)
    {
      stable_sort_function(begin(), end(), compare);
    }

    //*************************************************************************
    /// Sorts the indirect vector between two iterators.
    //*************************************************************************
    void stable_sort(iterator first, iterator last)
    {
      stable_sort_function(first, last, std::less<T>());
    }

    //*************************************************************************
    /// Sorts the indirect vector between two iterators.
    //*************************************************************************
    template <typename TCompare>
    void stable_sort(iterator first, iterator last, TCompare compare)
    {
      stable_sort_function(first, last, compare);
    }

    //*************************************************************************
    /// Checks to see if the vector is sorted, using the default 'less'.
    //*************************************************************************
    bool is_sorted() const
    {
      return is_sorted_function(cbegin(), cend(), std::less<T>());
    }

    //*************************************************************************
    /// Sorts the indirect vector using a supplied compare function.
    //*************************************************************************
    template <typename TCompare>
    bool is_sorted(TCompare compare) const
    {
      return is_sorted_function(cbegin(), cend(), compare);
    }

    //*************************************************************************
    /// Sorts the indirect vector between two iterators.
    //*************************************************************************
    bool is_sorted(const_iterator first, const_iterator last) const
    {
      return is_sorted_function(first, last, std::less<T>());
    }

    //*************************************************************************
    /// Sorts the indirect vector between two iterators.
    //*************************************************************************
    template <typename TCompare>
    bool is_sorted(const_iterator first, const_iterator last, TCompare compare) const
    {
      return is_sorted_function(first, last, compare);
    }

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    iindirect_vector(etl::ivector<T*>& lookup_, etl::ipool& storage_)
      : lookup(lookup_)
      , storage(storage_)
    {
    }

    //*********************************************************************
    /// Initialise the indirect_vector.
    //*********************************************************************
    void initialise()
    {
      iterator itr = begin();

      while (itr != end())
      {
        storage.destroy<T>(etl::addressof(*itr));
        ++itr;
      }

      lookup.clear();
    }

    //*********************************************************************
    /// Move from a container.
    //*********************************************************************
    void move_container(iindirect_vector&& other)
    {
      if (this != &other)
      {
        initialise();

        typename iindirect_vector<T>::iterator itr = other.begin();

        while (itr != other.end())
        {
          push_back(std::move(*itr));
          ++itr;
        }

        other.initialise();
      }
    }

    etl::ivector<T*>& lookup;
    etl::ipool&       storage;

  private:

    //*********************************************************************
    /// Sorts the range.
    //*********************************************************************
    template <typename TCompare>
    void sort_function(iterator first, iterator last, TCompare compare)
    {
      etl::sort(first.lookup_itr, last.lookup_itr, ObjectCompare<TCompare>(compare));
    }

    //*********************************************************************
    /// Sorts the range.
    /// Stable
    //*********************************************************************
    template <typename TCompare>
    void stable_sort_function(iterator first, iterator last, TCompare compare)
    {
      etl::stable_sort(first.lookup_itr, last.lookup_itr, ObjectCompare<TCompare>(compare));
    }

    //*********************************************************************
    /// Checks if the range is sorted.
    //*********************************************************************
    template <typename TCompare>
    bool is_sorted_function(const_iterator first, const_iterator last, TCompare compare) const
    {
      return etl::is_sorted(first.lookup_itr, last.lookup_itr, ObjectCompare<TCompare>(compare));
    }

    //*********************************************************************
    /// How to compare two objects via the lookup iterators.
    /// \tparam TCompare Type to compare two T objects.
    //*********************************************************************
    template <typename TCompare>
    struct ObjectCompare
    {
      ObjectCompare(TCompare compare_)
        : compare(compare_)
      {
      }

      bool operator ()(const T* lhs, const T* rhs) const
      {
        return compare(*lhs, *rhs);
      }

      TCompare compare;
    };

    // Disable copy construction.
    iindirect_vector(const iindirect_vector&) ETL_DELETE;

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_INDIRECT_VECTOR) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual
#else
  protected:
#endif
    ~iindirect_vector()
    {
      initialise();
    }
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator ==(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator !=(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Less than operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the first indirect_vector is lexicographically less than the second, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator <(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  //***************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the first indirect_vector is lexicographically greater than the second, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator >(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return (rhs < lhs);
  }

  //***************************************************************************
  /// Less than or equal operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the first indirect_vector is lexicographically less than or equal to the second, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator <=(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return !(lhs > rhs);
  }

  //***************************************************************************
  /// Greater than or equal operator.
  ///\param lhs Reference to the first indirect_vector.
  ///\param rhs Reference to the second indirect_vector.
  ///\return <b>true</b> if the first indirect_vector is lexicographically greater than or equal to the second, otherwise <b>false</b>
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  bool operator >=(const etl::iindirect_vector<T>& lhs, const etl::iindirect_vector<T>& rhs)
  {
    return !(lhs < rhs);
  }

  //***************************************************************************
  /// A indirect_vector implementation that uses a fixed size buffer.
  ///\tparam T The element type.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class indirect_vector : public etl::iindirect_vector<T>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    indirect_vector()
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
    }

    //*************************************************************************
    /// Constructor, with size.
    ///\param initial_size The initial size of the indirect_vector.
    //*************************************************************************
    explicit indirect_vector(size_t initial_size)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->resize(initial_size);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initial_size  The initial size of the indirect_vector.
    ///\param value        The value to fill the indirect_vector with.
    //*************************************************************************
    indirect_vector(size_t initial_size, typename etl::iindirect_vector<T>::parameter_t value)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->resize(initial_size, value);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    indirect_vector(TIterator first, TIterator last)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_NO_STL)
    //*************************************************************************
    /// Constructor, from an initializer_list.
    //*************************************************************************
    indirect_vector(std::initializer_list<T> init)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    indirect_vector(const indirect_vector& other)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->assign(other.begin(), other.end());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    indirect_vector& operator = (const indirect_vector& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    indirect_vector(indirect_vector&& other)
      : etl::iindirect_vector<T>(lookup_vector, storage_pool)
    {
      this->move_container(std::move(other));
    }

    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    indirect_vector& operator = (indirect_vector&& rhs)
    {
      this->move_container(std::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~indirect_vector()
    {
      this->clear();
    }

  private:

    etl::vector<T*, MAX_SIZE> lookup_vector;
    etl::pool<T, MAX_SIZE>    storage_pool;
  };

  //***************************************************************************
  /// A indirect_vector implementation that uses a fixed size buffer.
  /// The buffer is supplied on construction.
  ///\tparam T The element type.
  ///\ingroup indirect_vector
  //***************************************************************************
  template <typename T>
  class indirect_vector<T, 0> : public etl::iindirect_vector<T>
  {
  public:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    indirect_vector(etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
    }

    //*************************************************************************
    /// Constructor, with size.
    ///\param initial_size The initial size of the indirect_vector.
    //*************************************************************************
    explicit indirect_vector(size_t initial_size, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->resize(initial_size);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initial_size  The initial size of the indirect_vector.
    ///\param value        The value to fill the indirect_vector with.
    //*************************************************************************
    indirect_vector(size_t initial_size, typename etl::iindirect_vector<T>::parameter_t value, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->resize(initial_size, value);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    indirect_vector(TIterator first, TIterator last, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_NO_STL)
    //*************************************************************************
    /// Constructor, from an initializer_list.
    //*************************************************************************
    indirect_vector(std::initializer_list<T> init, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    indirect_vector(const indirect_vector& other, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->assign(other.begin(), other.end());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    indirect_vector& operator = (const indirect_vector& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    indirect_vector(indirect_vector&& other, etl::ivector<T*>& lookup_, etl::ipool& pool_)
      : etl::iindirect_vector<T>(lookup_, pool_)
    {
      ETL_ASSERT(lookup_.capacity() <= pool_.capacity(), ETL_ERROR(indirect_vector_buffer_missmatch));
      this->move_container(std::move(other));
    }

    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    indirect_vector& operator = (indirect_vector&& rhs)
    {
      this->move_container(std::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~indirect_vector()
    {
      this->clear();
    }
  };
}

#ifdef ETL_COMPILER_GCC
#pragma GCC diagnostic pop
#endif

#undef ETL_FILE

#endif

