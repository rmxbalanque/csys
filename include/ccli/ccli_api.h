
#ifndef CCLI_API_H
#define CCLI_API_H

#ifdef CCLI_COMPILED_LIB
#  undef CCLI_HEADER_ONLY
#  define CCLI_INLINE
#  ifndef CCLI_API

	 // Windows Shared Library.
#    if defined(_WIN32)
#    	ifdef ccli_EXPORTS
#    	  define CCLI_API __declspec(dllexport)
#    	else
#    	  define CCLI_API __declspec(dllimport)
#    	endif
#	 endif

	// Linux shared library.
#    ifdef ccli_EXPORTS
#      define CCLI_API __attribute__((visibility("default")))
#    else
#      define CCLI_API __attribute__((visibility("default")))
#    endif
#  endif

	// No export.
#  ifndef CCLI_NO_EXPORT
#    if defined(_WIN32)
#      define CCCLI_NO_EXPORT
#    else
#      define CCLI_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#  endif
#else
#  define CCLI_API
#  define CCLI_NO_EXPORT
#  define CCLI_HEADER_ONLY
#  define CCLI_INLINE inline
#endif

#if defined(__GNUC__) || defined(__clang__)
#define CCLI_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define CCLI_DEPRECATED __declspec(deprecated)
#else
#define CCLI_DEPRECATED
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
