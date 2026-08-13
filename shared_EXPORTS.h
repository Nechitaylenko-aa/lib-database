
#ifndef shared_EXPORTS_H
#define shared_EXPORTS_H

#ifdef SHARED_EXPORTS_BUILT_AS_STATIC
#  define shared_EXPORTS
#  define DATABASED_NO_EXPORT
#else
#  ifndef shared_EXPORTS
#    ifdef databased_EXPORTS
        /* We are building this library */
#      define shared_EXPORTS __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define shared_EXPORTS __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef DATABASED_NO_EXPORT
#    define DATABASED_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DATABASED_DEPRECATED
#  define DATABASED_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef DATABASED_DEPRECATED_EXPORT
#  define DATABASED_DEPRECATED_EXPORT shared_EXPORTS DATABASED_DEPRECATED
#endif

#ifndef DATABASED_DEPRECATED_NO_EXPORT
#  define DATABASED_DEPRECATED_NO_EXPORT DATABASED_NO_EXPORT DATABASED_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DATABASED_NO_DEPRECATED
#    define DATABASED_NO_DEPRECATED
#  endif
#endif

#endif /* shared_EXPORTS_H */
