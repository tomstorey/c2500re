#ifndef _STDINT_H_
#define _STDINT_H_

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef unsigned char uint8_t;

typedef long intptr_t;
typedef long ptrdiff_t;
typedef unsigned long uintptr_t;

typedef long long intmax_t;

/* 7.18.1.2 Minimum-width integer types */
/*! smallest signed integer type with width of at least 8 */
typedef int8_t int_least8_t;

/*! smallest signed integer type with width of at least 16 */
typedef int16_t int_least16_t;

/*! smallest signed integer type with width of at least 32 */
typedef int32_t int_least32_t;

/*! smallest signed integer type with width of at least 64 */
typedef int64_t int_least64_t;

/*! smallest unsigned integer type with width of at least 8 */
typedef uint8_t uint_least8_t;

/*! smallest unsigned integer type with width of at least 16 */
typedef uint16_t uint_least16_t;

/*! smallest unsigned integer type with width of at least 32 */
typedef uint32_t uint_least32_t;

/*! smallest unsigned integer type with width of at least 64 */
typedef uint64_t uint_least64_t;

/* 7.18.1.3 Fastest-width integer types */
/*! fastest signed integer type with width of at least 8 */
typedef int8_t int_fast8_t;

/*! fastest signed integer type with width of at least 16 */
typedef int32_t int_fast16_t;

/*! fastest signed integer type with width of at least 32 */
typedef int32_t int_fast32_t;

/*! fastest signed integer type with width of at least 64 */
typedef int64_t int_fast64_t;

/*! fastest unsigned integer type with width of at least 8 */
typedef uint8_t uint_fast8_t;

/*! fastest unsigned integer type with width of at least 16 */
typedef uint32_t uint_fast16_t;

/*! fastest unsigned integer type with width of at least 32 */
typedef uint32_t uint_fast32_t;

/*! fastest unsigned integer type with width of at least 64 */
typedef uint64_t uint_fast64_t;

/* 7.18.2.1 Limits of exact-width integer types */
/*! Expands to 127. Maximum value of an object of type int8_t */
#define INT8_MAX 127

/*! Expands to 32767. Maximum value of an object of type int16_t */
#define INT16_MAX 32767

/*! Expands to 2147483647. Maximum value of an object of type int32_t */
#define INT32_MAX 2147483647

/*! Expands to 9223372036854775807LL. Maximum value of an object of type int64_t */
#define INT64_MAX 9223372036854775807LL

/*! Expands to -128. Minimum value of an object of type int8_t */
#define INT8_MIN -128

/*! Expands to -32768. Minimum value of an object of type int16_t */
#define INT16_MIN -32768
/*
   Note:  the literal "most negative int" cannot be written in C --
   the rules in the standard (section 6.4.4.1 in C99) will give it
   an unsigned type, so INT32_MIN (and the most negative member of
   any larger signed type) must be written via a constant expression.
*/
/*! Expands to (-INT32_MAX - 1). Minimum value of an object of type int32_t */
#define INT32_MIN (-INT32_MAX - 1)

/*! Expands to (-INT64_MAX - 1). Minimum value of an object of type int64_t */
#define INT64_MIN (-INT64_MAX - 1)

/*! Expands to 255. Maximum value of an object of type uint8_t */
#define UINT8_MAX 255

/*! Expands to 65535. Maximum value of an object of type uint16_t */
#define UINT16_MAX 65535

/*! Expands to 4294967295U. Maximum value of an object of type uint32_t */
#define UINT32_MAX 4294967295U

/*! Expands to 18446744073709551615ULL. Maximum value of an object of type uint64_t */
#define UINT64_MAX 18446744073709551615ULL

/* 7.18.2.2 Limits of minimum-width integer types */
/*! Minimum value of an object of type int_least8_t */
#define INT_LEAST8_MIN INT8_MIN

/*! Minimum value of an object of type int_least16_t */
#define INT_LEAST16_MIN INT16_MIN

/*! Minimum value of an object of type int_least32_t */
#define INT_LEAST32_MIN INT32_MIN

/*! Minimum value of an object of type int_least64_t */
#define INT_LEAST64_MIN INT64_MIN

/*! Minimum value of an object of type uint_least8_t */
#define UINT_LEAST8_MIN UINT8_MIN

/*! Minimum value of an object of type uint_least16_t */
#define UINT_LEAST16_MIN UINT16_MIN

/*! Minimum value of an object of type uint_least32_t */
#define UINT_LEAST32_MIN UINT32_MIN

/*! Minimum value of an object of type uint_least64_t */
#define UINT_LEAST64_MIN UINT64_MIN

/*! Maximum value of an object of type uint_least8_t */
#define INT_LEAST8_MAX INT8_MAX

/*! Maximum value of an object of type int_least16_t */
#define INT_LEAST16_MAX INT16_MAX

/*! Maximum value of an object of type int_least32_t */
#define INT_LEAST32_MAX INT32_MAX

/*! Maximum value of an object of type int_least64_t */
#define INT_LEAST64_MAX INT64_MAX

/*! Maximum value of an object of type uint_least8_t */
#define UINT_LEAST8_MAX UINT8_MAX

/*! Maximum value of an object of type uint_least16_t */
#define UINT_LEAST16_MAX UINT16_MAX

/*! Maximum value of an object of type uint_least32_t */
#define UINT_LEAST32_MAX UINT32_MAX

/*! Maximum value of an object of type uint_least64_t */
#define UINT_LEAST64_MAX UINT64_MAX

/* 7.18.2.3 Limits of fastest minimum-width integer types */
/*! Minimum value of an object of type int_fast8_t */
#define INT_FAST8_MIN INT8_MIN

/*! Minimum value of an object of type int_fast16_t */
#define INT_FAST16_MIN INT32_MIN

/*! Minimum value of an object of type int_fast32_t */
#define INT_FAST32_MIN INT32_MIN

/*! Minimum value of an object of type int_fast64_t */
#define INT_FAST64_MIN INT64_MIN

/*! Maximum value of an object of type int_fast8_t */
#define INT_FAST8_MAX INT8_MAX

/*! Maximum value of an object of type int_fast16_t */
#define INT_FAST16_MAX INT32_MAX

/*! Maximum value of an object of type int_fast32_t */
#define INT_FAST32_MAX INT32_MAX

/*! Maximum value of an object of type int_fast64_t */
#define INT_FAST64_MAX INT64_MAX

/*! Minimum value of an object of type uint_fast8_t */
#define UINT_FAST8_MIN UINT8_MIN

/*! Minimum value of an object of type uint_fast16_t */
#define UINT_FAST16_MIN UINT32_MIN

/*! Minimum value of an object of type uint_fast32_t */
#define UINT_FAST32_MIN UINT32_MIN

/*! Minimum value of an object of type uint_fast64_t */
#define UINT_FAST64_MIN UINT64_MIN

/*! Maximum value of an object of type uint_fast8_t */
#define UINT_FAST8_MAX UINT8_MAX

/*! Maximum value of an object of type uint_fast16_t */
#define UINT_FAST16_MAX UINT32_MAX

/*! Maximum value of an object of type uint_fast32_t */
#define UINT_FAST32_MAX UINT32_MAX

/*! Maximum value of an object of type uint_fast64_t */
#define UINT_FAST64_MAX UINT64_MAX

#if __WORDSIZE == 64
/*! Minimum value of an object of type intptr_t */
#define INTPTR_MIN INT64_MIN

/*! Maximum value of an object of type intptr_t */
#define INTPTR_MAX INT64_MAX

/*! Maximum value of an object of type uintptr_t  */
#define UINTPTR_MAX UINT64_MAX

/*! Minimum value of an object of type ptrdiff_t */
#define PTRDIFF_MIN INT64_MIN

/*! Maximum value of an object of type ptrdiff_t */
#define PTRDIFF_MAX INT64_MAX

/*! Maximum value of object of size_t type */
#define SIZE_MAX UINT64_MAX

#else // __WORDSIZE is 32 bits

/*! Minimum value of an object of type intptr_t */
#define INTPTR_MIN INT32_MIN

/*! Maximum value of an object of type intptr_t */
#define INTPTR_MAX INT32_MAX

/*! Maximum value of an object of type uintptr_t  */
#define UINTPTR_MAX UINT32_MAX

/*! Minimum value of an object of type ptrdiff_t */
#define PTRDIFF_MIN INT32_MIN

/*! Maximum value of an object of type ptrdiff_t */
#define PTRDIFF_MAX INT32_MAX
#endif

/*! Minimum value of an object of type intmax_t */
#define INTMAX_MIN INT64_MIN

/*! Maximum value of an object of type intmax_t  */
#define INTMAX_MAX INT64_MAX

/*! Maximum value of an object of type uintmax_t  */
#define UINTMAX_MAX UINT64_MAX

/*! Maximum value of object of size_t type */
#ifndef SIZE_MAX
#define SIZE_MAX UINT32_MAX
#endif

#endif /* _STDINT_H_ */
