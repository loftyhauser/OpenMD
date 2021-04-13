/*
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:

 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

// See http://www.boost.org/libs/any for Documentation.

#ifndef BOOST_ANY_INCLUDED
#define BOOST_ANY_INCLUDED

// what:  variant type boost::any
// who:   contributed by Kevlin Henney,
//        with features contributed and bugs found by
//        Ed Brey, Mark Rodgers, Peter Dimov, and James Curran
// when:  July 2001
// where: tested with BCC 5.5, MSVC 6.0, and g++ 2.95

#include <algorithm>
#include <typeinfo>

namespace boost {

  template<class E>
  inline void throw_exception(E const& e) {
    throw e;
  }

  class any {
  public:  // structors
    any() : content(0) {}

    template<typename ValueType>
    any(const ValueType& value) : content(new holder<ValueType>(value)) {}

    any(const any& other) :
        content(other.content ? other.content->clone() : 0) {}

    ~any() { delete content; }

  public:  // modifiers
    any& swap(any& rhs) {
      std::swap(content, rhs.content);
      return *this;
    }

    template<typename ValueType>
    any& operator=(const ValueType& rhs) {
      any(rhs).swap(*this);
      return *this;
    }

    any& operator=(const any& rhs) {
      any(rhs).swap(*this);
      return *this;
    }

  public:  // queries
    bool empty() const { return !content; }

    const std::type_info& type() const {
      return content ? content->type() : typeid(void);
    }

    //#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    //    private: // types
    //#else
  public:  // types (public so any_cast can be non-friend)
    //#endif

    class placeholder {
    public:  // structors
      virtual ~placeholder() {}

    public:  // queries
      virtual const std::type_info& type() const = 0;

      virtual placeholder* clone() const = 0;
    };

    template<typename ValueType>
    class holder : public placeholder {
    public:  // structors
      holder(const ValueType& value) : held(value) {}

    public:  // queries
      virtual const std::type_info& type() const { return typeid(ValueType); }

      virtual placeholder* clone() const { return new holder(held); }

    public:  // representation
      ValueType held;
    };

    //#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    //
    //    private: // representation
    //
    //        template<typename ValueType>
    //        friend ValueType * any_cast(any *);
    //
    //#else
    //
  public:  // representation (public so any_cast can be non-friend)
    //
    //#endif

    placeholder* content;
  };

  class bad_any_cast : public std::bad_cast {
  public:
    virtual const char* what() const throw() {
      return "boost::bad_any_cast: "
             "failed conversion using boost::any_cast";
    }
  };

  template<typename ValueType>
  ValueType* any_cast(any* operand) {
    return operand && operand->type() == typeid(ValueType) ?
               &static_cast<any::holder<ValueType>*>(operand->content)->held :
               0;
  }

  template<typename ValueType>
  const ValueType* any_cast(const any* operand) {
    return any_cast<ValueType>(const_cast<any*>(operand));
  }

  template<typename ValueType>
  ValueType any_cast(const any& operand) {
    const ValueType* result = any_cast<ValueType>(&operand);
    if (!result) boost::throw_exception(bad_any_cast());
    return *result;
  }

  //  template<typename T>
  //  bool equal_any_type(const any& operand) {
  //
  //  }

}  // namespace boost

// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#endif
