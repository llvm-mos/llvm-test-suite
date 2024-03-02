#include <fixed_point.h>
#include <float.h>
#include <stdio.h>

// My extremely hardcoded print float method since there isn't %f print support
template<typename T>
void print_float(const char* name, T fp) {
  uint8_t fsize = fp.frac_bitcount();
  float fscale = (float) (1llu << fsize);
  auto f = fp.get();
  if ((fp.int_bitcount() + fsize) == 24 && !fp.is_signed()) {
    // If we are writing a 24bit number, the underlying type is 32 bit, so mask off the upper byte
    f &= 0xffffff;
  }
  float val = f / fscale;
  printf("%s %.02f\n", name, val);
}

template <bool Signed = true, typename T>
void print_hex(const char* name, T fp) {

  switch (sizeof(fp)) {
    case 1:
      if (Signed) {
        printf("%s %lX\n", name, (int8_t)fp.get());
      } else {
        printf("%s %lX\n", name, (uint8_t)fp.get());
      }
      break;
    case 2:
      if (Signed) {
        printf("%s %X\n", name, (int16_t)fp.get());
      } else {
        printf("%s %X\n", name, (uint16_t)fp.get());
      }
      break;
    case 3:
    case 4:
      if (Signed) {
        printf("%s %lX\n", name, (int32_t)fp.get());
      } else {
        printf("%s %lX\n", name, (uint32_t)fp.get());
      }
      break;
    default:
      break;
  }
}

int main(void) {

  ////////
  // Constructor tests

  // basic literal constructor
  using namespace fixedpoint_literals;
  f8_8 basic_constructor{100};
  printf("basic_constructor %d.%d\n", (char)basic_constructor.as_i(), (char)basic_constructor.as_f());

  // using a negative value in a signed class
  f8_8 signed_value{-100};
  print_float("signed_value", signed_value);

  // using an signed value with a negative number and storing to an unsigned is converted to the unsigned value
  fu8_8 unsigned_value{-100};
  printf("unsigned_value %hd.%hd\n", (uint16_t) unsigned_value.get() / 256, (uint8_t) ((((int16_t)unsigned_value.as_f()) * 256) / 100) );

  // floating point number constructor
  f8_8 from_float{10.9};
  print_float("from_float", from_float); // gets rounded a bit when printing so it says 10.89

  // copy constructors
  // basic copy constructor with same sizes
  f8_8 copy_same_size = from_float;
  print_float("copy_same_size", copy_same_size);

  // copy constructor from signed to unsigned type
  fu8_8 copy_of_signed_positive = basic_constructor;
  print_float("copy_of_signed_positive", copy_of_signed_positive);
  // copy constructor from negative signed number to unsigned treats the integer portion as unsigned
  fu8_8 copy_of_signed_negative = signed_value;
  print_float("copy_of_signed_negative", copy_of_signed_negative);
  // copy constructor with a larger int size (same float size)
  f16_8 copy_bigger_size = basic_constructor;
  print_float("copy_bigger_size", copy_bigger_size);

  // copy constructor with a signed larger int size sign extends
  f16_8 copy_bigger_signed = signed_value;
  print_float("copy_bigger_signed", copy_bigger_signed);
  print_hex("copy_bigger_signed", copy_bigger_signed); // verify that it gets sign extended

  // Copy constructor to an unsigned larger int with a signed number
  fu16_8 copy_bigger_unsigned = signed_value;
  print_float("copy_bigger_unsigned", copy_bigger_unsigned);
  print_hex("copy_bigger_unsigned", copy_bigger_unsigned);

  // Copy constructor with a smaller int size with a signed number truncates the upper bits
  f16_8 bigger_int_size = -800.8_16_8; // 
  f8_8 smaller_int_size = bigger_int_size.as<8, 8>();
  print_float("smaller_int_size", smaller_int_size); // truncates to df

  // Copy constructor with a different float size resizes the float value
  fu8_8 float_half_value = 12.5_u8_8;
  fu12_12 bigger_float_size = float_half_value.as<12, 12>();
  print_float("bigger_float_size", bigger_float_size);

  fu16_16 big_float_size = 18.7_u16_16;
  fu8_8 small_float_size = big_float_size.as<8,8,false>();
  print_float("small_float_size", small_float_size);


  ////////////////
  // Operator overloads

  f8_8 unary_minus_op = 100.6_8_8;
  unary_minus_op = -unary_minus_op;
  print_float("unary_minus_op", unary_minus_op);
  print_hex("unary_minus_op", unary_minus_op);

  f8_8 unary_negate_op = ~100.6_8_8;
  print_float("unary_negate_op", unary_negate_op);

  // 127.5 % 100.4 == 27.1
  f8_8 modulo_op = 127.5_8_8;
  print_float("modulo_op", modulo_op % 100.4_8_8);

  // 127.5 & 124.0 == 124.0
  f8_8 binary_and = 127.5_8_8;
  print_float("binary_and", binary_and & 124.0_8_8);

  // 2.5 | 8.0 == 10.5
  f8_8 binary_or = 2.5_8_8;
  print_float("binary_or", binary_or | 8.0_8_8);

  // 3.5 ^ 5.3 == 0x380 ^ 0x54c == 0x6cc (6.8)
  f8_8 binary_xor = 3.5_8_8;
  print_float("binary_xor", binary_xor ^ 5.3_8_8);

  ////////////////
  // Arithmetic operators

  f8_8 small_signed = -50.8_8_8;
  f16_8 large_signed = -400.3_16_8;
  fu8_8 small_unsigned = 200.1_u8_8;
  fu16_8 large_unsigned = 1000.6_u16_8;

  print_float("add int small signed", small_signed + 1);
  print_float("add fp small signed", small_signed + 1.1_8_8);
  print_float("add int small unsigned", small_unsigned + 1);
  print_float("add fp small unsigned", small_unsigned + 1.1_u8_8);
  // Overflow wraps around and doesn't extend the int type
  print_float("add fp small signed overflow", small_signed + -100.1_8_8);
  print_float("add fp small unsigned overflow", small_unsigned + 100.1_u8_8);
  // Adding large type + small type returns large type
  print_float("add fp large + small signed", large_signed + small_signed);
  print_float("add fp large + small unsigned", large_unsigned + small_unsigned);
  // Adding small type + large type promotes to the larger type
  print_float("add fp small + large signed", small_signed + large_signed);
  print_float("add fp small + large unsigned", small_unsigned + large_unsigned);

  print_float("sub int small signed", small_signed - 1);
  print_float("sub fp small signed", small_signed - 1.1_8_8);
  print_float("sub int small unsigned", small_unsigned - 1);
  print_float("sub fp small unsigned", small_unsigned - 1.1_u8_8);
  print_float("sub fp small signed overflow", small_signed - -100.1_8_8);
  print_float("sub fp small unsigned overflow", small_unsigned - 100.1_u8_8);
  print_float("sub fp large - small signed", large_signed - small_signed);
  print_float("sub fp large - small unsigned", large_unsigned - small_unsigned);
  print_float("sub fp small - large signed", small_signed - large_signed);
  auto out = small_unsigned - large_unsigned;
  print_float("sub fp small - large unsigned", out);
  print_hex("sub fp small - large unsigned", out);

  // Multiplication checks
  auto signval = -10.8_8_8;
  auto unsignval = 10.8_8_8;
  print_float("mul int small signed", signval * 3);
  print_float("mul fp small signed", signval * 3.3_8_8);
  print_float("mul int overflow signed", small_signed * 3);
  print_float("mul fp overflow signed", small_signed * 3.3_8_8);
  print_float("mul int small signed", unsignval * 3);
  print_float("mul fp small signed", unsignval * 3.3_8_8);
  print_float("mul int overflow signed", small_unsigned * 3);
  print_float("mul fp overflow signed", small_unsigned * 3.3_8_8);

  // Comparison checks
  printf("sign < unsigned %s\n", (-10.0_8_8 < 10.0_8_8) ? "true" : "false");
  printf("sign > unsigned %s\n", (-10.0_8_8 > 10.0_8_8) ? "true" : "false");
  printf("sign == unsigned %s\n", (-10.0_8_8 == 10.0_8_8) ? "true" : "false");
  printf("large sign < unsigned %s\n", (-1000.0_16_8 < 10.0_8_8) ? "true" : "false");

  return 0;
}
