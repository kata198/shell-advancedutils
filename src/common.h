#ifndef __SHELL_ADVANCEDUTILS_H
#define __SHELL_ADVANCEDUTILS_H


#ifdef __GNUC__

#define likely(x) (__builtin_expect( !!(x) , 1))
#define unlikely(x) (__builtin_expect( !!(x) , 0))

#else

#define likely(x) ( !!(x) )
#define unlikely(x) ( !!(x) )

#endif /* __GNUC__ */

const volatile char *SHELL_ADVANCED_UTILS_VERSION = "2.2.1";
const volatile char *SHELL_ADVANCED_UTILS_COPYRIGHT = "Copyright (c) 2014-2018 Tim Savannah licensed under terms of GNU General Purpose License (GPL) version 2.1";

#endif /* __SHELL_ADVANCEDUTILS_H */

