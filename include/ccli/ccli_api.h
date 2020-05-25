
#ifndef CCLI_API_H
#define CCLI_API_H

#ifdef CCLI_BUILD_STATIC
#  define CCLI_API
#  define CCLI_NO_EXPORT
#else
#  ifndef CCLI_API
#    ifdef ccli_EXPORTS
        /* We are building this library */
#      define CCLI_API 
#    else
        /* We are using this library */
#      define CCLI_API 
#    endif
#  endif

#  ifndef CCLI_NO_EXPORT
#    define CCLI_NO_EXPORT 
#  endif
#endif

#ifndef CCLI_DEPRECATED
#  define CCLI_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef CCLI_DEPRECATED_EXPORT
#  define CCLI_DEPRECATED_EXPORT CCLI_API CCLI_DEPRECATED
#endif

#ifndef CCLI_DEPRECATED_NO_EXPORT
#  define CCLI_DEPRECATED_NO_EXPORT CCLI_NO_EXPORT CCLI_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CCLI_NO_DEPRECATED
#    define CCLI_NO_DEPRECATED
#  endif
#endif

#endif /* CCLI_API_H */
