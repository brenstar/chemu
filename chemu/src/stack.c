
#include "chemu/stack.h"

#ifdef __GNUC__
#ifdef _INLINE_
#include "inline.h"
#else
#define INLINE 
#endif
#include "chemu/stack.ipp"
#endif

#if !defined(_INLINE_) && defined(_MSC_VER)
#define INLINE
#include "chemu/stack.ipp"
#endif


