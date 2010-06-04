// file      : odb/meta/remove-c.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_META_REMOVE_C_HXX
#define ODB_META_REMOVE_C_HXX

namespace odb
{
  namespace meta
  {
    template <typename X>
    struct remove_c
    {
      typedef X r;
    };

    template <typename X>
    struct remove_c<X const>
    {
      typedef X r;
    };
  }
}

#endif // ODB_META_REMOVE_C_HXX
