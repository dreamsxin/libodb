// file      : odb/shared-ptr-traits.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SHARED_PTR_TRAITS_HXX
#define ODB_SHARED_PTR_TRAITS_HXX

#include <new>     // operators new/delete
#include <cstddef> // std::size_t

#include <odb/shared-ptr.hxx>

namespace odb
{
  template <typename P>
  class shared_ptr_traits;

  // Default implementation that should work for any sensible smart
  // pointer with one template argument (object type). The only
  // assumptions that we make are the availability of operator-> and
  // operator*, and that the former does not throw if the pointer is
  // NULL.
  //
  template <typename T, template <typename> class P>
  class shared_ptr_traits< P<T> >
  {
  public:
    typedef T type;
    typedef P<T> shared_ptr;

    // Return underlying pointer, including NULL.
    //
    static type*
    get_ptr (const shared_ptr& p)
    {
      return p.operator-> ();
    }

    // Return reference to the pointed-to object.
    //
    static type&
    get_ref (const shared_ptr& p)
    {
      return *p;
    }

    // Return true if the pointer is NULL.
    //
    static bool
    null_ptr (const shared_ptr& p)
    {
      return get_ptr () == 0;
    }

  public:
    // Allocate memory for a shared object.
    //
    static void*
    allocate (std::size_t n)
    {
      return operator new (n);
    }

    // Free memory allocated for a shared object. This functions is
    // only called if the constructor of the object being created
    // fails. Otherwise, shared_ptr is used to delete the object
    // and free the memory. This behavior is identical to the one
    // used by operator delete overloading.
    //
    static void
    free (void* p)
    {
      operator delete (p);
    }
  };

  // Specialization for odb::shared_ptr.
  //
  template <typename T>
  class shared_ptr_traits< shared_ptr<T> >
  {
  public:
    typedef T type;
    typedef odb::shared_ptr<T> shared_ptr;

    static type*
    get_ptr (const shared_ptr& p)
    {
      return p.get ();
    }

    static type&
    get_ref (const shared_ptr& p)
    {
      return *p;
    }

    static bool
    null_ptr (const shared_ptr& p)
    {
      return !p;
    }

    static void*
    allocate (std::size_t n)
    {
      return operator new (n, shared);
    }

    static void
    free (void* p)
    {
      operator delete (p, shared);
    }
  };
}

#endif // ODB_SHARED_PTR_TRAITS_HXX
