// file      : odb/transaction.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>
#include <odb/transaction.hxx>

namespace odb
{
  //
  // transaction
  //

  static transaction* current_transaction = 0;

  transaction::
  transaction (transaction_impl* impl)
      : finilized_ (false), impl_ (impl)
  {
    current_transaction = this;
  }

  transaction::
  ~transaction ()
  {
    if (!finilized_)
    {
      try
      {
        rollback ();
      }
      catch (const database_exception&)
      {
        // Ignore it.
      }
    }

    current_transaction = 0;
    delete impl_;
  }

  bool transaction::
  has_current ()
  {
    return current_transaction != 0;
  }

  transaction& transaction::
  current ()
  {
    if (current_transaction == 0)
      throw not_in_transaction ();

    return *current_transaction;
  }

  void transaction::
  commit ()
  {
    if (finilized_)
      throw transaction_already_finilized ();

    // Flush the session if we are in the session-per-transaction mode.
    //
    if (impl_->own_session ())
      session ().flush ();

    impl_->commit ();
    finilized_ = true;
  }

  void transaction::
  rollback ()
  {
    if (finilized_)
      throw transaction_already_finilized ();

    finilized_ = true;
    impl_->rollback ();
  }

  //
  // transaction_impl
  //

  transaction_impl::
  ~transaction_impl ()
  {
    if (own_session ())
      reinterpret_cast<session_type*> (&session_mem_)->~session ();
  }
}
