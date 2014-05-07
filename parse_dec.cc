#include <stdio.h>
#include <stdint.h>

// Parses the string p as a decimal number. Leading 0s are allowed. Overflow
// and underflow is detected properly. A leading '-' is allowed iff T is a
// signed integral type. A leading "+' is not allowed. Leading whitespace is
// not allowed. -0 is allowed.
//
// Call it with T as an integral type, otherwise it doesn't compile.
template<class T>bool parse_dec(char const *p, T *out) {
  // ~(T)~(T)0 compiles for integral types, but it doesn't compile for pointer
  // or floating point types etc.
  const bool is_signed = (T)~(T)0 < (T)1;
  const T nmax = (T)(is_signed ? ~((T)1 << (sizeof(T) * 8 - 1)) : ~(T)0) / 10;
  bool is_negative = false;
  if (is_signed && *p == '-') {
    is_negative = true;
    ++p;
  }
  const char cmax = is_signed ? (is_negative ? 8 : 7) : 5;
  T n = 0;
  do {
    if (*p + 0U - '0' > 9U) return false;  // Not a digit.
    const char c = *p++ - '0';
    if (!(n < nmax || (n == nmax && c <= cmax))) {
      return false;  // Overflow.
    }
    n = 10 * n + c;
  } while (*p != '\0');
  *out = is_negative ? -n : n;
  return true;  // Success.
}

int main(int argc, char **argv) {
  (void)argc;
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;
  int8_t i8;
  int16_t i16;
  int32_t i32;
  int64_t i64;
  // Doesn't compile, good: double d; parse_dec("0", &d);
  // Doesn't compile, good: parse_dec("0", argv);
  for (++argv; *argv; ++argv) {
    bool b;
    printf("\narg=%s\n", *argv);
    u8 = 0; b = parse_dec(*argv, &u8);
    printf("bu8=%d u8=%u\n", b, u8);    
    u16 = 0; b = parse_dec(*argv, &u16);
    printf("bu16=%d u16=%u\n", b, u16);
    u32 = 0; b = parse_dec(*argv, &u32);
    printf("bu32=%d u32=%llu\n", b, (unsigned long long)u32);
    u64 = 0; b = parse_dec(*argv, &u64);
    printf("bu64=%d u64=%llu\n", b, (unsigned long long)u64);
    i8 = 0; b = parse_dec(*argv, &i8);
    printf("bi8=%d i8=%d\n", b, i8);    
    i16 = 0; b = parse_dec(*argv, &i16);
    printf("bi16=%d i16=%d\n", b, i16);
    i32 = 0; b = parse_dec(*argv, &i32);
    printf("bi32=%d i32=%lld\n", b, (long long)i32);
    i64 = 0; b = parse_dec(*argv, &i64);
    printf("bi64=%d i64=%lld\n", b, (long long)i64);
  }
  return 0;
}
