
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* the name of <hash_map> */
#define HASH_MAP_CLASS 

/* the location of <unordered_map> or <hash_map> */
#define HASH_MAP_H 

/* the namespace of hash_map/hash_set */
#define HASH_NAMESPACE 

/* the name of <hash_set> */
#define HASH_SET_CLASS 

/* the location of <unordered_set> or <hash_set> */
#define HASH_SET_H 

/* define if the compiler has hash_map */
/* #undef HAVE_HASH_MAP */

/* define if the compiler has hash_set */
/* #undef HAVE_HASH_SET */

/* Define if you have POSIX threads libraries and header files. */
/* #undef HAVE_PTHREAD */

/* Enable classes using zlib compression. */
#undef HAVE_ZLIB

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# undef _ALL_SOURCE
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# undef _GNU_SOURCE
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# undef _POSIX_PTHREAD_SEMANTICS
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# undef _TANDEM_SOURCE
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# undef __EXTENSIONS__
#endif


/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
#undef _POSIX_1_SOURCE

/* Define to 1 if you need to in order for `stat' and other things to work. */
#undef _POSIX_SOURCE

#endif /* _CONFIG_H_ */

