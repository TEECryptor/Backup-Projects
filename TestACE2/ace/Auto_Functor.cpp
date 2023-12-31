// Auto_Functor.cpp,v 1.2 2005/10/28 16:14:51 ossama Exp

#ifndef ACE_AUTO_FUNCTOR_CPP
#define ACE_AUTO_FUNCTOR_CPP

#include "ace/Auto_Functor.h"

#if !defined(__ACE_INLINE__)
# include "ace/Auto_Functor.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename X, typename Functor>
ACE_Utils::Auto_Functor<X,Functor>::~Auto_Functor()
  ACE_THROW_SPEC (())
{
  reset(0);
}

template<typename X, typename Functor> void
ACE_Utils::Auto_Functor<X,Functor>::reset(X * p)
  ACE_THROW_SPEC (())
{
  if(p_ != 0)
  {
    f_(p_);
  }
  p_ = p;
}

template<typename X, typename Functor>void
ACE_Utils::Auto_Functor<X,Functor>::reset(X * p, Functor f)
  ACE_THROW_SPEC (())
{
  reset(p);
  f_ = f;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /*ACE_AUTO_FUNCTOR_CPP*/
