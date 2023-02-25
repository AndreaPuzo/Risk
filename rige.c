#include "rige.h"
#include <stdlib.h>

/* ----------------------------------------------------------------
 * mem
 */

_RIGE_API ptr_t mem_alloc (usiz_t size)
{
  if (0 == size)
    return RIGE_NULL ;

  /* maybe later a platform-specific implementation */
  return malloc(size) ;
}

_RIGE_API ptr_t mem_calloc (usiz_t n, usiz_t size)
{
  if (0 == n || 0 == size)
    return RIGE_NULL ;

  /* maybe later a platform-specific implementation */
  return calloc(n, size) ;
}

_RIGE_API ptr_t mem_realloc (ptr_t ptr, usiz_t size)
{
  if (0 == size) {
    if (RIGE_NULL != ptr) {
      free(ptr) ;
    }

    return RIGE_NULL ;
  }

  if (RIGE_NULL == ptr)
    return malloc(size) ;

  /* maybe later a platform-specific implementation */
  return realloc(ptr, size) ;
}

_RIGE_API void mem_dealloc (ptr_t ptr)
{
  if (RIGE_NULL != ptr) {
    /* maybe later a platform-specific implementation */
    free(ptr) ;
  }
}

_RIGE_API usiz_t mem_copy (ptr_t _dst, const ptr_t _src, usiz_t n)
{
  u8_t * dst = (u8_t *)_dst ;
  u8_t * src = (u8_t *)_src ;

  if (RIGE_NULL == dst || RIGE_NULL == src)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size < n ; ++size)
    dst[size] = src[size] ;

  return size ;
}

_RIGE_API usiz_t mem_move (ptr_t _dst, const ptr_t _src, usiz_t n)
{
  u8_t * dst = (u8_t *)_dst ;
  u8_t * src = (u8_t *)_src ;

  if (RIGE_NULL == dst || RIGE_NULL == src)
    return RIGE_NPOS ;

  /* it is not possible to implement `memmove` efficiently in standard C.
   * see this post: https://stackoverflow.com/questions/4023320/how-to-implement-memmove-in-standard-c-without-an-intermediate-copy
   */

  /* create a temp. heap region */
  u8_t * tmp = (u8_t *)mem_alloc(n * sizeof(u8_t)) ;

  if (RIGE_NULL == tmp)
    return RIGE_NULL ;

  usiz_t size ;

  /* copy `src` into `tmp` */
  for (size = 0 ; size < n ; ++size) {
    tmp[size] = src[size] ;
  }

  /* copy `tmp` into `dst` */
  for (size = 0 ; size < n ; ++size) {
    dst[size] = tmp[size] ;
  }

  /* remember to free `tmp` */
  mem_dealloc(tmp) ;

  return size ;
}

_RIGE_API usiz_t mem_set (ptr_t _ptr, i32_t chr, usiz_t n)
{
  u8_t * ptr = (u8_t *)_ptr ;

  if (RIGE_NULL == ptr)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size < n ; ++size)
    ptr[size] = chr ;

  return size ;
}

_RIGE_API i32_t mem_comp (const ptr_t _lhs, const ptr_t _rhs, usiz_t n)
{
  u8_t * lhs = (u8_t *)_lhs ;
  u8_t * rhs = (u8_t *)_rhs ;

  if (RIGE_NULL == lhs || RIGE_NULL == rhs)
    return -1 ;

  usiz_t size ;

  for (size = 0 ; size < n && lhs[size] == rhs[size] ; ++size)
    /* continue */ ;

  return (i32_t)lhs[size] - (i32_t)rhs[size] ;
}

_RIGE_API usiz_t mem_for_each (const ptr_t _ptr, usiz_t n, i32_t (* pred) (i32_t))
{
  u8_t * ptr = (u8_t *)_ptr ;

  if (RIGE_NULL == ptr || RIGE_NULL == pred)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size < n && 0 != pred(ptr[size]) ; ++size)
    /* continue */ ;

  return size ;
}

_RIGE_API u32_t mem_hash_djb2 (const ptr_t _ptr, usiz_t n)
{
  u8_t * ptr = (u8_t *)_ptr ;

  if (RIGE_NULL == ptr)
    return RIGE_NPOS ;

  /* djb2 magic number */
  u32_t retval = 5381 ;
  usiz_t size ;

  for (size = 0 ; size < n ; ++size) {
    /* it works like `retval * 33 ^ ptr[size]`*/
    retval = ((retval << 5) + retval) ^ ptr[size] ;
  }

  /* you can write something like this
   * `retval % <something>`
   */
  return retval ;
}

/* ----------------------------------------------------------------
 * chr
 */

#define _I_CNTRL     0x0001
#define _I_PRINT     0x0002
#define _I_SPACE_HOR 0x0004
#define _I_SPACE_VER 0x0008
#define _I_PUNCT     0x0010
#define _I_GRAPH     0x0020
#define _I_UPPER     0x0040
#define _I_LOWER     0x0080
#define _I_DIGIT_BIN 0x0100
#define _I_DIGIT_OCT 0x0200
#define _I_DIGIT_DEC 0x0400
#define _I_DIGIT_HEX 0x0800

i32_t _chr_tab [] = {
  /*          .0       .1       .2       .3       .4       .5       .6       .7       .8       .9       .A       .B       .C       .D       .E       .F          */
  /* 0. */ 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0005 , 0x0009 , 0x0009 , 0x0009 , 0x0009 , 0x0001 , 0x0001 , /* 0. */
  /* 1. */ 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , 0x0001 , /* 1. */
  /* 2. */ 0x0004 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , /* 2. */
  /* 3. */ 0x0F02 , 0x0F02 , 0x0E02 , 0x0E02 , 0x0E02 , 0x0E02 , 0x0E02 , 0x0E02 , 0x0C02 , 0x0C02 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , /* 3. */
  /* 4. */ 0x0032 , 0x0862 , 0x0862 , 0x0862 , 0x0862 , 0x0862 , 0x0862 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , /* 4. */
  /* 5. */ 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0062 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , /* 5. */
  /* 6. */ 0x0032 , 0x08A2 , 0x08A2 , 0x08A2 , 0x08A2 , 0x08A2 , 0x08A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , /* 6. */
  /* 7. */ 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x00A2 , 0x0032 , 0x0032 , 0x0032 , 0x0032 , 0x0001   /* 7. */
} ;

#define _verify_prop(chr, prop)     \
  (_chr_tab[(chr) & 0x7F] & (prop))

_RIGE_API i32_t chr_is_ascii (i32_t chr)
{
  return 0 == (chr >> 7) ;
}

_RIGE_API i32_t chr_to_ascii (i32_t chr)
{
  return chr & 0x7F ;
}

_RIGE_API i32_t chr_is_ansi (i32_t chr)
{
  return 0 == (chr >> 8) ;
}

_RIGE_API i32_t chr_to_ansi (i32_t chr)
{
  return chr & 0xFF ;
}

_RIGE_API i32_t chr_is_cntrl (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_CNTRL) ;
}

_RIGE_API i32_t chr_is_print (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_PRINT) ;
}

_RIGE_API i32_t chr_is_space_hor (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_SPACE_HOR) ;
}

_RIGE_API i32_t chr_is_space_ver (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_SPACE_VER) ;
}

_RIGE_API i32_t chr_is_space (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_SPACE_HOR | _I_SPACE_VER) ;
}

_RIGE_API i32_t chr_is_punct (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_PUNCT) ;
}

_RIGE_API i32_t chr_is_graph (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_PUNCT | _I_DIGIT_DEC | _I_UPPER | _I_LOWER) ;
}

_RIGE_API i32_t chr_is_upper (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_UPPER) ;
}

_RIGE_API i32_t chr_is_lower (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_LOWER) ;
}

_RIGE_API i32_t chr_to_upper (i32_t chr)
{
  i32_t type = chr_is_lower(chr) ;

  return ((0 == type) * chr) | ((0 != type) * (chr & ~0x20)) ;
}

_RIGE_API i32_t chr_to_lower (i32_t chr)
{
  i32_t type = chr_is_upper(chr) ;

  return ((0 == type) * chr) | ((0 != type) * (chr | 0x20)) ;
}

_RIGE_API i32_t chr_is_alpha (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_UPPER | _I_LOWER) ;
}

_RIGE_API i32_t chr_is_digit (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_DIGIT_DEC) ;
}

_RIGE_API i32_t chr_is_digit_bin (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_DIGIT_BIN) ;
}

_RIGE_API i32_t chr_is_digit_oct (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_DIGIT_OCT) ;
}

_RIGE_API i32_t chr_is_digit_hex (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_DIGIT_HEX) ;
}

_RIGE_API i32_t chr_is_alnum (i32_t chr)
{
  return 0 != _verify_prop(chr, _I_DIGIT_DEC | _I_UPPER | _I_LOWER) ;
}

_RIGE_API i32_t chr_to_digit (i32_t chr, i32_t base)
{
  chr_t digits_upper [] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
  chr_t digits_lower [] = "0123456789abcdefghijklmnopqrstuvwxyz" ;

  usiz_t digit ;

  if (0 == base) {
    digit = cstr_chr(digits_upper, chr) ;

    if (sizeof(digits_upper) <= digit) {
      digit = cstr_chr(digits_lower /* could be `+ 10` a better choice? */, chr) ;

      if (sizeof(digits_lower) <= digit)
        return -1 ;
    }
  } else if (0 < base) {
    digit = cstr_chr(digits_upper, chr) ;

    if (base <= digit)
      return -1 ;
  } else {
    digit = cstr_chr(digits_lower, chr) ;

    if (base <= digit)
      return -1 ;
  }

  return digit ;
}

#undef _I_CNTRL
#undef _I_PRINT
#undef _I_SPACE_HOR
#undef _I_SPACE_VER
#undef _I_PUNCT
#undef _I_GRAPH
#undef _I_UPPER
#undef _I_LOWER
#undef _I_DIGIT_BIN
#undef _I_DIGIT_OCT
#undef _I_DIGIT_DEC
#undef _I_DIGIT_HEX

#undef _verify_prop

/* ----------------------------------------------------------------
 * cstr
 */

_RIGE_API usiz_t cstr_size (cstr_t cstr)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; 0 != cstr[size] ; ++size)
    /* continue */ ;

  return size ;
}

_RIGE_API usiz_t cstr_n_size (cstr_t cstr, usiz_t n)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size < n && 0 != cstr[size] ; ++size)
    /* continue */ ;

  return size ;
}

_RIGE_API usiz_t cstr_copy (cstr_t dst, const cstr_t src)
{
  if (RIGE_NULL == dst || RIGE_NULL == src)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; 0 != (dst[size] = src[size]) ; ++size)
    /* continue */ ;

  return size ;
}

_RIGE_API usiz_t cstr_n_copy (cstr_t dst, const cstr_t src, usiz_t n)
{
  if (RIGE_NULL == dst || RIGE_NULL == src)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size < n && 0 != (dst[size] = src[size]) ; ++size)
    /* continue */ ;

  return size ;
}

_RIGE_API i32_t cstr_comp (const cstr_t lhs, const cstr_t rhs)
{
  if (RIGE_NULL == lhs || RIGE_NULL == rhs)
    return -1 ;

  usiz_t size ;

  for (size = 0 ; 0 != lhs[size] && lhs[size] == rhs[size] ; ++size)
    /* continue */ ;

  return (i32_t)lhs[size] - (i32_t)rhs[size] ;
}

_RIGE_API i32_t cstr_n_comp (const cstr_t lhs, const cstr_t rhs, usiz_t n)
{
  if (RIGE_NULL == lhs || RIGE_NULL == rhs)
    return -1 ;

  usiz_t size ;

  for (size = 0 ; size < n && 0 != lhs[size] && lhs[size] == rhs[size] ; ++size)
    /* continue */ ;

  return (i32_t)lhs[size] - (i32_t)rhs[size] ;
}

_RIGE_API i32_t cstr_icomp (const cstr_t lhs, const cstr_t rhs)
{
  if (RIGE_NULL == lhs || RIGE_NULL == rhs)
    return -1 ;

  usiz_t size = 0 ;

  /* this variables have to be initialized because
   * of the type of loop I choose
   */
  i32_t lhs_chr = chr_to_lower(lhs[size]) ;
  i32_t rhs_chr = chr_to_lower(rhs[size]) ;

  /* could be another type of loop a better choice? */
  for (; 0 != lhs[size] ; ++size) {
    lhs_chr = chr_to_lower(lhs[size]) ;
    rhs_chr = chr_to_lower(rhs[size]) ;

    if (lhs_chr != rhs_chr)
      break ;
  }

  return lhs_chr - rhs_chr ;
}

_RIGE_API i32_t cstr_n_icomp (const cstr_t lhs, const cstr_t rhs, usiz_t n)
{
  if (RIGE_NULL == lhs || RIGE_NULL == rhs)
    return -1 ;

  usiz_t size = 0 ;

  /* this variables have to be initialized because
   * of the type of loop I choose
   */
  i32_t lhs_chr = chr_to_lower(lhs[size]) ;
  i32_t rhs_chr = chr_to_lower(rhs[size]) ;

  /* could be another type of loop a better choice? */
  for (; size < n && 0 != lhs[size] ; ++size) {
    lhs_chr = chr_to_lower(lhs[size]) ;
    rhs_chr = chr_to_lower(rhs[size]) ;

    if (lhs_chr != rhs_chr)
      break ;
  }

  return lhs_chr - rhs_chr ;
}

_RIGE_API usiz_t cstr_chr (const cstr_t cstr, chr_t chr)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; 0 != cstr[size] ; ++size) {
    if (chr == cstr[size])
      return size ;
  }

  return RIGE_NPOS ;
}

_RIGE_API usiz_t cstr_n_chr (const cstr_t cstr, chr_t chr, usiz_t n)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size < n && 0 != cstr[size] ; ++size) {
    if (chr == cstr[size])
      return size ;
  }

  return RIGE_NPOS ;
}

_RIGE_API usiz_t cstr_ichr (const cstr_t cstr, chr_t chr)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  usiz_t size ;

  chr = chr_to_lower(chr) ;

  for (size = 0 ; 0 != cstr[size] ; ++size) {
    if (chr == chr_to_lower(cstr[size]))
      return size ;
  }

  return RIGE_NPOS ;
}

_RIGE_API usiz_t cstr_n_ichr (const cstr_t cstr, chr_t chr, usiz_t n)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  usiz_t size ;

  chr = chr_to_lower(chr) ;

  for (size = 0 ; size < n && 0 != cstr[size] ; ++size) {
    if (chr == chr_to_lower(cstr[size]))
      return size ;
  }

  return RIGE_NPOS ;
}

_RIGE_API usiz_t cstr_str (const cstr_t cstr, const cstr_t str)
{
  if (RIGE_NULL == cstr || RIGE_NULL == str)
    return RIGE_NPOS ;

  usiz_t str_size = cstr_size(str) ;

  if (0 == str_size)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; 0 != cstr[size] ; ++size) {
    if (0 == cstr_n_comp(cstr + size, str, str_size))
      return size ;
  }

  return RIGE_NPOS ;
}

_RIGE_API usiz_t cstr_n_str (const cstr_t cstr, const cstr_t str, usiz_t n)
{
  if (RIGE_NULL == cstr || RIGE_NULL == str)
    return RIGE_NPOS ;

  usiz_t str_size = cstr_size(str) ;

  if (0 == str_size || n < str_size)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size + str_size <= n && 0 != cstr[size] ; ++size) {
    if (0 == cstr_n_comp(cstr + size, str, str_size))
      return size ;
  }

  return RIGE_NPOS ;
}

_RIGE_API usiz_t cstr_istr (const cstr_t cstr, const cstr_t str)
{
  if (RIGE_NULL == cstr || RIGE_NULL == str)
    return RIGE_NPOS ;

  usiz_t str_size = cstr_size(str) ;

  if (0 == str_size)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; 0 != cstr[size] ; ++size) {
    if (0 == cstr_n_icomp(cstr + size, str, str_size))
      return size ;
  }

  return RIGE_NPOS ;
}

_RIGE_API usiz_t cstr_n_istr (const cstr_t cstr, const cstr_t str, usiz_t n)
{
  if (RIGE_NULL == cstr || RIGE_NULL == str)
    return RIGE_NPOS ;

  usiz_t str_size = cstr_size(str) ;

  if (0 == str_size || n < str_size)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size + str_size <= n && 0 != cstr[size] ; ++size) {
    if (0 == cstr_n_icomp(cstr + size, str, str_size))
      return size ;
  }

  return RIGE_NPOS ;
}

_RIGE_API usiz_t cstr_to_upper (cstr_t cstr)
{
  if (RIGE_NULL == cstr)
    return 0 ;

  usiz_t size ;

  for (size = 0 ; 0 != cstr[size] ; ++size)
    cstr[size] = chr_to_upper(cstr[size]) ;

  return size ;
}

_RIGE_API usiz_t cstr_n_to_upper (cstr_t cstr, usiz_t n)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size < n && 0 != cstr[size] ; ++size)
    cstr[size] = chr_to_upper(cstr[size]) ;

  return size ;
}

_RIGE_API usiz_t cstr_to_lower (cstr_t cstr)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; 0 != cstr[size] ; ++size)
    cstr[size] = chr_to_lower(cstr[size]) ;

  return size ;
}

_RIGE_API usiz_t cstr_n_to_lower (cstr_t cstr, usiz_t n)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size < n && 0 != cstr[size] ; ++size)
    cstr[size] = chr_to_lower(cstr[size]) ;

  return size ;
}

_RIGE_API usiz_t cstr_for_each (const cstr_t cstr, i32_t (* pred) (i32_t))
{
  if (RIGE_NULL == cstr || RIGE_NULL == pred)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; 0 != cstr[size] && 0 != pred(cstr[size]) ; ++size)
    /* continue */ ;

  return size ;
}

_RIGE_API usiz_t cstr_n_for_each (const cstr_t cstr, usiz_t n, i32_t (* pred) (i32_t))
{
  if (RIGE_NULL == cstr || RIGE_NULL == pred)
    return RIGE_NPOS ;

  usiz_t size ;

  for (size = 0 ; size < n && 0 != cstr[size] && 0 != pred(cstr[size]) ; ++size)
    /* continue */ ;

  return size ;
}

_RIGE_API cstr_t cstr_dup (const cstr_t cstr)
{
  return cstr_n_dup(cstr, cstr_size(cstr)) ;
}

_RIGE_API cstr_t cstr_n_dup (const cstr_t cstr, usiz_t n)
{
  cstr_t dupstr = (cstr_t)mem_calloc(n + 1, sizeof(chr_t)) ;

  if (RIGE_NULL != dupstr) {
    /* `cstr` could be `RIGE_NULL` */
    if (n == cstr_n_copy(dupstr, cstr, n)) {
      dupstr[n] = 0 ;
    } else {
      mem_dealloc(dupstr) ;
      dupstr = RIGE_NULL ;
    }
  }

  return dupstr ;
}

_RIGE_API u32_t cstr_hash_djb2 (const cstr_t cstr)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  /* djb2 magic number */
  u32_t retval = 5381 ;
  usiz_t size ;

  for (size = 0 ; 0 != cstr[size] ; ++size) {
    /* it works like `retval * 33 ^ cstr[size]`*/
    retval = ((retval << 5) + retval) ^ cstr[size] ;
  }

  /* you can write something like this
   * `retval % <something>`
   */
  return retval ;
}

_RIGE_API u32_t cstr_n_hash_djb2 (const cstr_t cstr, usiz_t n)
{
  if (RIGE_NULL == cstr)
    return RIGE_NPOS ;

  /* djb2 magic number */
  u32_t retval = 5381 ;
  usiz_t size ;

  for (size = 0 ; size < n && 0 != cstr[size] ; ++size) {
    /* it works like `retval * 33 ^ cstr[size]`*/
    retval = ((retval << 5) + retval) ^ cstr[size] ;
  }

  /* you can write something like this
   * `retval % <something>`
   */
  return retval ;
}

/* ----------------------------------------------------------------
 * str
 */

_RIGE_API str_t str_make (const cstr_t cstr)
{
  return str_n_make(cstr, cstr_size(cstr)) ;
}

_RIGE_API str_t str_n_make (const cstr_t cstr, usiz_t n)
{
  str_t str ;

  str.size = n ;
  str.data = cstr_n_dup(cstr, n) ;
#ifdef _RIGE_HAS_HASH_STRING
  str.hash = cstr_n_hash_djb2(cstr, n) ;
#endif

  return str ;
}

_RIGE_API usiz_t str_copy (str_t * dst, const str_t * src)
{
  if (RIGE_NULL == dst || RIGE_NULL == src)
    return RIGE_NPOS ;

  str_t tmp = str_n_make(src->data, src->size) ;

  if (RIGE_NULL != dst->data) {
    mem_dealloc(dst->data) ;
  }

  *dst = tmp ;

  return dst->size ;
}

_RIGE_API usiz_t str_n_copy (str_t * dst, const str_t * src, usiz_t n)
{
  if (RIGE_NULL == dst || RIGE_NULL == src)
    return RIGE_NPOS ;

  if (src->size < n) {
    n = src->size ;
  }

  str_t tmp = str_n_make(src->data, n) ;

  if (RIGE_NULL != dst->data) {
    mem_dealloc(dst->data) ;
  }

  *dst = tmp ;

  return dst->size ;
}

_RIGE_API i32_t str_comp (const str_t * lhs, const cstr_t rhs)
{
  if (RIGE_NULL == lhs || lhs->size != cstr_size(rhs))
    return -1 ;

#ifdef _RIGE_HAS_HASH_STRING
  if (lhs->hash != cstr_hash_djb2(rhs))
    return -1 ;
#endif

  return cstr_n_comp(lhs->data, rhs, lhs->size) ;
}

_RIGE_API i32_t str_n_comp (const str_t * lhs, const cstr_t rhs, usiz_t n)
{
  if (RIGE_NULL == lhs)
    return -1 ;

  if (lhs->size < n) {
    n = lhs->size ;
  }

#ifdef _RIGE_HAS_HASH_STRING
  if (cstr_n_hash_djb2(lhs->data, n) != cstr_n_hash_djb2(rhs, n))
    return -1 ;
#endif

  return cstr_n_comp(lhs->data, rhs, n) ;
}