#ifndef _RIGE_H
# define _RIGE_H

# define _RIGE_API

# include <stdint.h>
# include <stdarg.h>
# include <stdio.h>
# include <math.h>

# define RIGE_VERSION_MAJOR 0
# define RIGE_VERSION_MINOR 0
# define RIGE_VERSION_PATCH 0

# define RIGE_VERSION            \
  (                              \
    (RIGE_VERSION_MAJOR << 24) | \
    (RIGE_VERSION_MINOR << 16) | \
    (RIGE_VERSION_PATCH <<  0)   \
  )

typedef void *    ptr_t  ;
typedef char      chr_t  ;
typedef uint8_t   u8_t   ;
typedef uint16_t  u16_t  ;
typedef uint32_t  u32_t  ;
typedef uint64_t  u64_t  ;
typedef int8_t    i8_t   ;
typedef int16_t   i16_t  ;
typedef int32_t   i32_t  ;
typedef int64_t   i64_t  ;
typedef uintptr_t uptr_t ;
typedef intptr_t  iptr_t ;
typedef uptr_t    usiz_t ;
typedef iptr_t    isiz_t ;

# define RIGE_NULL ((ptr_t)0)
# define RIGE_NPOS ((usiz_t)-1)

_RIGE_API ptr_t mem_alloc (usiz_t size) ;
_RIGE_API ptr_t mem_calloc (usiz_t n, usiz_t size) ;
_RIGE_API ptr_t mem_realloc (ptr_t ptr, usiz_t size) ;
_RIGE_API void mem_dealloc (ptr_t ptr) ;
_RIGE_API usiz_t mem_copy (ptr_t dst, const ptr_t src, usiz_t n) ;
_RIGE_API usiz_t mem_move (ptr_t dst, const ptr_t src, usiz_t n) ;
_RIGE_API usiz_t mem_set (ptr_t ptr, i32_t chr, usiz_t n) ;
_RIGE_API i32_t mem_comp (const ptr_t lhs, const ptr_t rhs, usiz_t n) ;
_RIGE_API usiz_t mem_for_each (const ptr_t ptr, usiz_t n, i32_t (* pred) (i32_t)) ;

_RIGE_API i32_t chr_is_ascii (i32_t chr) ;
_RIGE_API i32_t chr_to_ascii (i32_t chr) ;
_RIGE_API i32_t chr_is_ansi (i32_t chr) ;
_RIGE_API i32_t chr_to_ansi (i32_t chr) ;
_RIGE_API i32_t chr_is_cntrl (i32_t chr) ;
_RIGE_API i32_t chr_is_print (i32_t chr) ;
_RIGE_API i32_t chr_is_space_hor (i32_t chr) ;
_RIGE_API i32_t chr_is_space_ver (i32_t chr) ;
_RIGE_API i32_t chr_is_space (i32_t chr) ;
_RIGE_API i32_t chr_is_punct (i32_t chr) ;
_RIGE_API i32_t chr_is_graph (i32_t chr) ;
_RIGE_API i32_t chr_is_upper (i32_t chr) ;
_RIGE_API i32_t chr_is_lower (i32_t chr) ;
_RIGE_API i32_t chr_to_upper (i32_t chr) ;
_RIGE_API i32_t chr_to_lower (i32_t chr) ;
_RIGE_API i32_t chr_is_alpha (i32_t chr) ;
_RIGE_API i32_t chr_is_digit (i32_t chr) ;
_RIGE_API i32_t chr_is_digit_bin (i32_t chr) ;
_RIGE_API i32_t chr_is_digit_oct (i32_t chr) ;
_RIGE_API i32_t chr_is_digit_hex (i32_t chr) ;
_RIGE_API i32_t chr_is_alnum (i32_t chr) ;
_RIGE_API i32_t chr_to_digit (i32_t chr, i32_t base) ;

typedef chr_t * cstr_t ;

_RIGE_API usiz_t cstr_size (cstr_t cstr) ;
_RIGE_API usiz_t cstr_n_size (cstr_t cstr, usiz_t n) ;
_RIGE_API usiz_t cstr_copy (cstr_t dst, const cstr_t src) ;
_RIGE_API usiz_t cstr_n_copy (cstr_t dst, const cstr_t src, usiz_t n) ;
_RIGE_API i32_t cstr_comp (const cstr_t lhs, const cstr_t rhs) ;
_RIGE_API i32_t cstr_n_comp (const cstr_t lhs, const cstr_t rhs, usiz_t n) ;
_RIGE_API i32_t cstr_icomp (const cstr_t lhs, const cstr_t rhs) ;
_RIGE_API i32_t cstr_n_icomp (const cstr_t lhs, const cstr_t rhs, usiz_t n) ;
_RIGE_API usiz_t cstr_chr (const cstr_t cstr, chr_t chr) ;
_RIGE_API usiz_t cstr_n_chr (const cstr_t cstr, chr_t chr, usiz_t n) ;
_RIGE_API usiz_t cstr_ichr (const cstr_t cstr, chr_t chr) ;
_RIGE_API usiz_t cstr_n_ichr (const cstr_t cstr, chr_t chr, usiz_t n) ;
_RIGE_API usiz_t cstr_str (const cstr_t cstr, const cstr_t str) ;
_RIGE_API usiz_t cstr_n_str (const cstr_t cstr, const cstr_t str, usiz_t n) ;
_RIGE_API usiz_t cstr_istr (const cstr_t cstr, const cstr_t str) ;
_RIGE_API usiz_t cstr_n_istr (const cstr_t cstr, const cstr_t str, usiz_t n) ;
_RIGE_API usiz_t cstr_to_upper (cstr_t cstr) ;
_RIGE_API usiz_t cstr_n_to_upper (cstr_t cstr, usiz_t n) ;
_RIGE_API usiz_t cstr_to_lower (cstr_t cstr) ;
_RIGE_API usiz_t cstr_n_to_lower (cstr_t cstr, usiz_t n) ;
_RIGE_API usiz_t cstr_for_each (const cstr_t cstr, i32_t (* pred) (i32_t)) ;
_RIGE_API usiz_t cstr_n_for_each (const cstr_t cstr, usiz_t n, i32_t (* pred) (i32_t)) ;
_RIGE_API cstr_t cstr_dup (const cstr_t cstr) ;
_RIGE_API cstr_t cstr_n_dup (const cstr_t cstr, usiz_t n) ;
_RIGE_API u32_t cstr_hash_djb2 (const cstr_t cstr) ;
_RIGE_API u32_t cstr_n_hash_djb2 (const cstr_t cstr, usiz_t n) ;

typedef struct str_s str_t ;

struct str_s {
  chr_t * data ;
  usiz_t  size ;
# ifdef _RIGE_HAS_HASH_STRING
  u32_t   hash ;
# endif
} ;

_RIGE_API str_t str_make (const cstr_t cstr) ;
_RIGE_API str_t str_n_make (const cstr_t cstr, usiz_t n) ;
_RIGE_API usiz_t str_copy (str_t * dst, const str_t * src) ;
_RIGE_API usiz_t str_n_copy (str_t * dst, const str_t * src, usiz_t n) ;
_RIGE_API i32_t str_comp (const str_t * lhs, const cstr_t rhs) ;
_RIGE_API i32_t str_n_comp (const str_t * lhs, const cstr_t rhs, usiz_t n) ;

# define RIGE_VEC_DECL(_type)                \
  typedef struct _type ## v_s _type ## v_t ; \
                                             \
  struct _type ## v_s {                      \
    _type ## _t x ;                          \
    _type ## _t y ;                          \
  } ;

RIGE_VEC_DECL(u8)
RIGE_VEC_DECL(u16)
RIGE_VEC_DECL(u32)
RIGE_VEC_DECL(u64)
RIGE_VEC_DECL(i8)
RIGE_VEC_DECL(i16)
RIGE_VEC_DECL(i32)
RIGE_VEC_DECL(i64)

# define vec_set(_x, _y) { (_x)            , (_y)            }
# define vec_add(_a, _b) { (_a).x + (_b).x , (_a).y + (_b).y }
# define vec_sub(_a, _b) { (_a).x - (_b).x , (_a).y - (_b).y }
# define vec_mul(_a, _k) { (_a).x * (_k)   , (_a).y * (_k)   }
# define vec_div(_a, _k) { (_a).x / (_k)   , (_a).y / (_k)   }
# define vec_dot(_a, _b) ( (_a).x * (_b).x + (_a).y * (_b).y )
# define vec_norm(_a)    ( sqrt(vec_dot((_a), (_a))) )

#endif