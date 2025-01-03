/* libtiff/tif_config.h.  Generated from tif_config.h.in by configure.  */
/* libtiff/tif_config.h.in.  Generated from configure.ac by autoheader.  */

/* Support CCITT Group 3 & 4 algorithms */
#define CCITT_SUPPORT 1

/* Pick up YCbCr subsampling info from the JPEG data stream to support files
   lacking the tag (default enabled). */
#define CHECK_JPEG_YCBCR_SUBSAMPLING 1

/* enable partial strip reading for large strips (experimental) */
#undef CHUNKY_STRIP_READ_SUPPORT

/* Treat extra sample as alpha (default enabled). The RGBA interface will
   treat a fourth sample with no EXTRASAMPLE_ value as being ASSOCALPHA. Many
   packages produce RGBA files but don't mark the alpha properly. */
#define DEFAULT_EXTRASAMPLE_AS_ALPHA 1

/* enable deferred strip/tile offset/size loading (experimental) */
#undef DEFER_STRILE_LOAD

/* Define to 1 if you have the <assert.h> header file. */
#define HAVE_ASSERT_H

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H

/* Define as 0 or 1 according to the floating point format suported by the
   machine */
#define HAVE_IEEEFP 1

/* Define to 1 if you have the <io.h> header file. */
#define HAVE_IO_H

/* Define to 1 if you have the `jbg_newlen' function. */
/* #undef HAVE_JBG_NEWLEN */

/* Define to 1 if you have the `mmap' function. */
/* #undef HAVE_MMAP */

/* Define to 1 if you have the <search.h> header file. */
#define HAVE_SEARCH_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H

/* Define to 1 if you have the <unistd.h> header file. */
#ifndef HAVE_UNISTD_H
/* #undef HAVE_UNISTD_H */
#endif

/* Native cpu byte order: 1 if big-endian (Motorola) or 0 if little-endian
   (Intel) */
#define HOST_BIGENDIAN 0

/* Set the native cpu bit order (FILLORDER_LSB2MSB or FILLORDER_MSB2LSB) */
#define HOST_FILLORDER FILLORDER_LSB2MSB

/* Support ISO JBIG compression (requires JBIG-KIT library) */
#undef JBIG_SUPPORT

/* 8/12 bit libjpeg dual mode enabled */
/* #undef JPEG_DUAL_MODE_8_12 */

/* Support JPEG compression (requires IJG JPEG library) */
#undef JPEG_SUPPORT

/* 12bit libjpeg primary include file with path */
/* #undef LIBJPEG_12_PATH */

/* Support LogLuv high dynamic range encoding */
#define LOGLUV_SUPPORT 1

/* Support LZMA2 compression */
/* #undef LZMA_SUPPORT */

/* Support LZW algorithm */
#define LZW_SUPPORT 1

/* Support Microsoft Document Imaging format */
#define MDI_SUPPORT 1

/* Support NeXT 2-bit RLE algorithm */
#define NEXT_SUPPORT 1

/* Support Old JPEG compresson (read-only) */
#undef OJPEG_SUPPORT

/* Support Macintosh PackBits algorithm */
#define PACKBITS_SUPPORT 1

/* Support Pixar log-format algorithm (requires Zlib) */
#define PIXARLOG_SUPPORT 1

/* The size of `unsigned long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG 8

/* Support strip chopping (whether or not to convert single-strip uncompressed
   images to mutiple strips of specified size to reduce memory usage) */
#define STRIPCHOP_DEFAULT TIFF_STRIPCHOP

/* Default size of the strip in bytes (when strip chopping enabled) */
//#define STRIP_SIZE_DEFAULT 8192

/* Support ThunderScan 4-bit RLE algorithm */
#define THUNDER_SUPPORT 1

/* Signed 8-bit type */
#define TIFF_INT8_T signed char

/* Signed 16-bit type */
#define TIFF_INT16_T signed short

/* Signed 32-bit type */
#define TIFF_INT32_T signed int

/* Signed size type */
#define TIFF_SSIZE_T size_t

/* Unsigned 8-bit type */
#define TIFF_UINT8_T unsigned char

/* Unsigned 16-bit type */
#define TIFF_UINT16_T unsigned short

/* Unsigned 32-bit type */
#define TIFF_UINT32_T unsigned int

#ifdef _MSC_VER
/* Signed 64-bit type */
# define TIFF_INT64_T signed __int64
/* Unsigned 64-bit type */
# define TIFF_UINT64_T unsigned __int64
#else
/* Signed 64-bit type */
# define TIFF_INT64_T long long
/* Signed 64-bit type */
# define TIFF_UINT64_T unsigned long long
#endif

/* Signed 64-bit type formatter */
/* Unsigned 64-bit type formatter */
#if defined _MSC_VER || defined __MINGW__ || defined __MINGW32__
# define TIFF_UINT64_FORMAT "%I64u"
# define TIFF_SSIZE_FORMAT "%Iu"
#else
# define TIFF_UINT64_FORMAT "%llu"
# define TIFF_SSIZE_FORMAT "%zd"
#endif

/* define to use win32 IO system */
#define USE_WIN32_FILEIO

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
/* #undef WORDS_BIGENDIAN */

/* Support Deflate compression */
#define ZIP_SUPPORT 1
