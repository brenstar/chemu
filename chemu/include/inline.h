// Contains the INLINE definition
// On MSVC (Windows) its value is __inline
// On GCC (Unix) its value is inline

#ifndef _INLINE_H
#define _INLINE_H

#ifdef _MSC_VER
#define INLINE __inline
#else
#define INLINE inline
#endif


#endif
