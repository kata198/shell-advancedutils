#ifndef __SHELL_ADVANCEDUTILS_H
#define __SHELL_ADVANCEDUTILS_H


#ifdef __GNUC__

#define likely   (x) (__builtin_expect( !!(x) , 1))
#define unlikely (x) (__builtin_expect( !!(x) , 0))

#else

#define likely   (x) ( !!(x) )
#define unlikely (x) ( !!(x) )

#endif /* __GNUC__ */

#endif /* __SHELL_ADVANCEDUTILS_H */

