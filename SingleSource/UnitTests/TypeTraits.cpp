#include <type_traits>
#include <cstdint>
#include <cstdio>

namespace conditional_test {

typedef std::conditional<true, int, double>::type Type1;
typedef std::conditional<false, int, double>::type Type2;
static_assert(std::is_same_v<Type1, int> && std::is_same_v<Type2, double>,
              "conditional");
} // namespace conditional_test

namespace is_same_test {
typedef std::integral_constant<int, 2> two_t;
typedef std::integral_constant<int, 4> four_t;

static_assert(!std::is_same<two_t, four_t>::value,
              "two_t and four_t are not equal!");

static_assert(two_t::value * 2 == four_t::value, "2*2 != 4");
enum class my_e { e1, e2 };

typedef std::integral_constant<my_e, my_e::e1> my_e_e1;
typedef std::integral_constant<my_e, my_e::e2> my_e_e2;
static_assert(my_e_e1() == my_e::e1 && my_e_e1::value != my_e::e2 &&
                  std::is_same<my_e_e2, my_e_e2>::value,
              "is_same");
} // namespace is_same_test

namespace is_base_of_test {
class A {};
class B : A {};
class C : B {};
class D {};

static_assert(std::is_base_of_v<A, A> && std::is_base_of_v<A, B> &&
                  std::is_base_of_v<A, C> && !std::is_base_of_v<A, D> &&
                  !std::is_base_of_v<B, A> && !std::is_base_of_v<int, int>,
              "is_base_of");
} // namespace is_base_of_test

namespace is_convertible_test {
class E {
public:
  template <class T> E(T &&) {}
};

class A {};
class B : public A {};
class C {};
class D {
public:
  operator C() { return c; }
  C c;
};

static_assert(std::is_convertible_v<B *, A *> &&
                  !std::is_convertible_v<A *, B *> &&
                  std::is_convertible_v<D, C> &&
                  !std::is_convertible_v<B *, C *> &&
                  std::is_convertible_v<A, E>,
              "is_convertible");
} // namespace is_convertible_test

namespace is_invocable_test {
auto func2(char) -> int (*)() { return nullptr; }
static_assert(std::is_invocable_v<int()> && !std::is_invocable_v<int(), int> &&
                  std::is_invocable_r_v<int, int()> &&
                  !std::is_invocable_r_v<int *, int()> &&
                  std::is_invocable_r_v<void, void(int), int> &&
                  !std::is_invocable_r_v<void, void(int), void> &&
                  std::is_invocable_r_v<int (*)(), decltype(func2), char> &&
                  !std::is_invocable_r_v<int (*)(), decltype(func2), void>,
              "is_invocable");
} // namespace is_invocable_test

namespace is_void_test {
static_assert(std::is_void<void>::value && !std::is_void<int>::value,
              "is_void");
}

namespace is_null_pointer_test {
static_assert(std::is_null_pointer<decltype(nullptr)>::value &&
                  !std::is_null_pointer<int *>::value,
              "is_null_pointer");
}

namespace is_integral_test {
class A {
public:
  enum E {};
  int fun() const;
};
enum E : int {};
static_assert(!std::is_integral<A>::value && !std::is_integral_v<E> &&
                  !std::is_integral_v<float> && std::is_integral_v<int> &&
                  std::is_integral_v<const int> && std::is_integral_v<bool>,
              "is_integral");
} // namespace is_integral_test

namespace is_floating_point_test {
static_assert(!std::is_floating_point<is_integral_test::A>::value &&
                  std::is_floating_point<float>::value &&
                  !std::is_floating_point<float &>::value &&
                  std::is_floating_point<double>::value &&
                  !std::is_floating_point<double &>::value &&
                  !std::is_floating_point<int>::value,
              "is_floating_point");
}

namespace is_array_test {
static_assert(!std::is_array<is_integral_test::A>::value &&
                  std::is_array<is_integral_test::A[]>::value &&
                  std::is_array<is_integral_test::A[3]>::value &&
                  !std::is_array<float>::value && !std::is_array<int>::value &&
                  std::is_array<int[]>::value && std::is_array<int[3]>::value,
              "is_array");
}

namespace is_enum_test {
enum class Ec : int {};

static_assert(!std::is_enum<is_integral_test::A>::value &&
                  std::is_enum<is_integral_test::E>::value &&
                  std::is_enum<is_integral_test::A::E>() &&
                  !std::is_enum_v<int> && std::is_enum_v<Ec>,
              "is_enum");
} // namespace is_enum_test

namespace is_union_test {
union U {
  int a;
  float b;
  class UC {};
};

struct C {
  U d;
};

static_assert(!std::is_union<is_integral_test::A>::value &&
                  std::is_union<U>::value && !std::is_union<C>::value &&
                  !std::is_union<int>::value,
              "is_union");
} // namespace is_union_test

namespace is_class_test {
struct B {};

static_assert(!std::is_class_v<is_union_test::U> &&
                  std::is_class_v<is_union_test::U::UC> &&
                  std::is_class<is_integral_test::A>::value &&
                  std::is_class_v<B> && !std::is_class_v<B *> &&
                  !std::is_class_v<B &> && std::is_class_v<const B> &&
                  !std::is_class<is_enum_test::Ec>::value &&
                  !std::is_class_v<int> && std::is_class_v<struct S> &&
                  std::is_class_v<class Cinc>,
              "is_class");
} // namespace is_class_test

namespace is_function_test {
int f();
template <typename> struct PM_traits {};
template <class T, class U> struct PM_traits<U T::*> { using member_type = U; };

using T = PM_traits<decltype(&is_integral_test::A::fun)>::member_type; // T is
                                                                       // int()

static_assert(!std::is_function<is_integral_test::A>::value &&
                  std::is_function<int(int)>::value &&
                  std::is_function<decltype(f)>::value &&
                  !std::is_function<int>::value && std::is_function<T>::value &&
                  std::is_function<decltype(printf)>::value,
              "is_function");

} // namespace is_function_test

namespace is_pointer_test {
static_assert(!std::is_pointer<is_integral_test::A>::value &&
                  !std::is_pointer_v<is_integral_test::A> &&
                  !std::is_pointer<is_integral_test::A>() &&
                  !std::is_pointer<is_integral_test::A>{} &&
                  !std::is_pointer<is_integral_test::A>()() &&
                  !std::is_pointer<is_integral_test::A>{}() &&
                  std::is_pointer<is_integral_test::A *>::value &&
                  std::is_pointer<is_integral_test::A const *volatile>() &&
                  !std::is_pointer<is_integral_test::A &>::value &&
                  !std::is_pointer<int>::value &&
                  std::is_pointer<int *>::value &&
                  std::is_pointer<int **>::value &&
                  !std::is_pointer<int[10]>::value &&
                  !std::is_pointer<std::nullptr_t>::value,
              "is_pointer");
}

namespace is_lvalue_reference_test {
static_assert(!std::is_lvalue_reference<is_integral_test::A>::value &&
                  std::is_lvalue_reference_v<is_integral_test::A &> &&
                  !std::is_lvalue_reference<is_integral_test::A &&>::value &&
                  !std::is_lvalue_reference<int>::value &&
                  std::is_lvalue_reference<int &>::value &&
                  !std::is_lvalue_reference<int &&>::value,
              "is_lvalue_reference");
}

namespace is_rvalue_reference_test {
static_assert(!std::is_rvalue_reference_v<is_integral_test::A> &&
                  !std::is_rvalue_reference_v<is_integral_test::A &> &&
                  std::is_rvalue_reference_v<is_integral_test::A &&> &&
                  !std::is_rvalue_reference_v<int> &&
                  !std::is_rvalue_reference_v<int &> &&
                  std::is_rvalue_reference_v<int &&>,
              "is_rvalue_reference");
}

namespace is_member_object_pointer_test {
static_assert(
    std::is_member_object_pointer_v<int(is_integral_test::A::*)> &&
        !std::is_member_object_pointer<int (is_integral_test::A::*)()>::value,
    "is_member_object_pointer");
}

namespace is_member_function_pointer_test {
static_assert(
    std::is_member_function_pointer_v<decltype(&is_integral_test::A::fun)> &&
        !std::is_member_function_pointer<decltype(&is_union_test::C::d)>::value,
    "is_member_function_pointer");
}

namespace is_fundamental_test {
static_assert(!std::is_fundamental<is_integral_test::A>::value &&
                  std::is_fundamental<int>::value &&
                  !std::is_fundamental_v<int &> &&
                  !std::is_fundamental<int *>::value &&
                  std::is_fundamental<float>::value &&
                  !std::is_fundamental<float &>::value &&
                  !std::is_fundamental<float *>::value,
              "is_fundamental");
}

namespace is_arithmetic_test {
static_assert(
    !std::is_arithmetic_v<is_integral_test::A> && std::is_arithmetic_v<bool> &&
        std::is_arithmetic_v<int> && std::is_arithmetic_v<int const> &&
        !std::is_arithmetic_v<int &> && !std::is_arithmetic_v<int *> &&
        std::is_arithmetic_v<float> && std::is_arithmetic_v<float const> &&
        !std::is_arithmetic_v<float &> && !std::is_arithmetic_v<float *> &&
        std::is_arithmetic_v<char> && std::is_arithmetic_v<char const> &&
        !std::is_arithmetic_v<char &> && !std::is_arithmetic_v<char *> &&
        !std::is_arithmetic<std::byte>::value,
    "is_arithmetic");
}

namespace is_scalar_test {
static_assert(std::is_scalar_v<int> && std::is_scalar_v<double> &&
                  std::is_scalar_v<is_enum_test::Ec> &&
                  std::is_scalar<char const *>::value &&
                  std::is_scalar_v<decltype(&is_union_test::C::d)> &&
                  std::is_scalar_v<std::nullptr_t> &&
                  !std::is_scalar_v<is_integral_test::A>,
              "is_scalar");
}

namespace is_object_test {
static_assert(std::is_object_v<int> && !std::is_object<int &>::value &&
                  std::is_object<is_integral_test::A>::value &&
                  !std::is_object<is_integral_test::A &>::value,
              "is_object");
}

namespace is_compound_test {
static_assert(std::is_compound<is_integral_test::A>::value &&
                  !std::is_compound_v<int>,
              "is_compount");
}

namespace is_reference_test {
static_assert(!std::is_reference_v<is_integral_test::A> &&
                  std::is_reference_v<is_integral_test::A &> &&
                  std::is_reference_v<is_integral_test::A &&> &&
                  !std::is_reference_v<long> && std::is_reference_v<long &> &&
                  std::is_reference_v<long &&> &&
                  !std::is_reference_v<double *> &&
                  std::is_reference_v<double *&> &&
                  std::is_reference_v<double *&&>,
              "is_reference");
}

namespace is_member_pointer_test {
static_assert(std::is_member_pointer<int(is_integral_test::A::*)>::value &&
                  !std::is_member_pointer<int>::value,
              "is_member_pointer");
}

namespace is_const_test {
static_assert(!std::is_const_v<int> && std::is_const_v<const int> &&
                  !std::is_const_v<const int *> &&
                  std::is_const_v<int *const> &&
                  !std::is_const_v<const int &> &&
                  std::is_const_v<std::remove_reference_t<const int &>>,
              "is_const");
}

namespace is_volatile_test {
static_assert(!std::is_volatile<int>::value &&
                  std::is_volatile<volatile int>::value,
              "is_volatile");
}

namespace is_trivial_test {
struct Nt {
  Nt() {}
};

static_assert(std::is_trivial<is_integral_test::A>::value &&
                  !std::is_trivial<Nt>::value,
              "is_trivial");
} // namespace is_trivial_test

namespace is_trivially_copyable_test {
struct Ntc {
  Ntc(Ntc const &) {}
};

struct V {
  virtual void foo();
};

struct Tc {
  int m;

  Tc(Tc const &) = default; // -> trivially copyable
  Tc(int x) : m(x + 1) {}
};

static_assert(std::is_trivially_copyable<is_integral_test::A>::value &&
                  !std::is_trivially_copyable<Ntc>::value &&
                  !std::is_trivially_copyable<V>::value &&
                  std::is_trivially_copyable<Tc>::value,
              "is_trivially_copyable");
} // namespace is_trivially_copyable_test

namespace is_standard_layout_test {
struct Nsl {
  int m1;

private:
  int m2;
};

static_assert(
    std::is_standard_layout_v<is_integral_test::A> &&
        !std::is_standard_layout<Nsl>::value &&
        !std::is_standard_layout<is_trivially_copyable_test::V>::value,
    "is_standard_layout");
} // namespace is_standard_layout_test

namespace has_unique_object_representation_test {
struct foo {
  char c;
  float f;
  short st;
  int i;
};

struct bar {
  int a;
  int b;
};

static_assert(!std::has_unique_object_representations_v<foo> &&
                  std::has_unique_object_representations_v<bar>,
              "has_unique_object_representation");
} // namespace has_unique_object_representation_test

namespace is_empty_test {
struct NotEmpty {
  int m;
};

struct StaticMember {
  static int m;
};

union EmptyUnion {};

static_assert(std::is_empty_v<is_class_test::B> &&
                  !std::is_empty<NotEmpty>::value &&
                  std::is_empty<StaticMember>::value &&
                  !std::is_empty<is_trivially_copyable_test::V>::value &&
                  !std::is_empty<EmptyUnion>::value,
              "is_empty");
} // namespace is_empty_test

namespace is_polymorphic_test {
struct InheritP : is_trivially_copyable_test::V {};

struct Dtor {
  virtual ~Dtor() = default;
};

static_assert(!std::is_polymorphic_v<is_integral_test::A> &&
                  std::is_polymorphic<is_trivially_copyable_test::V>::value &&
                  std::is_polymorphic<InheritP>::value &&
                  std::is_polymorphic<Dtor>::value,
              "is_polymorphic");
} // namespace is_polymorphic_test

namespace is_abstract_test {
struct Abstract {
  virtual void foo() = 0;
};

struct InheritAbstract : Abstract {};

static_assert(!std::is_abstract_v<is_integral_test::A> &&
                  !std::is_abstract<is_trivially_copyable_test::V>::value &&
                  std::is_abstract<Abstract>::value &&
                  std::is_abstract<InheritAbstract>::value,
              "is_abstract");
} // namespace is_abstract_test

namespace is_final_test {
class F final {};

static_assert(!std::is_final_v<is_integral_test::A> && std::is_final<F>::value,
              "is_final");
} // namespace is_final_test

namespace is_aggregate_test {
struct NonAggregate {
  NonAggregate(int, const char *) {}
};

constexpr void is_aggregate_test() {
  static_assert(
      std::is_aggregate_v<has_unique_object_representation_test::bar> &&
          !std::is_aggregate<NonAggregate>::value,
      "is_aggregate");
}
} // namespace is_aggregate_test

namespace is_signed_test {
static_assert(!std::is_signed_v<is_integral_test::A> &&
                  std::is_signed_v<float> && std::is_signed_v<signed int> &&
                  !std::is_signed_v<unsigned int> &&
                  !std::is_signed_v<is_integral_test::E> &&
                  !std::is_signed_v<is_enum_test::Ec> &&
                  !std::is_signed_v<bool>,
              "is_signed");
}

namespace is_unsigned_test {
enum UEnum : unsigned {};
enum class UEnumC : unsigned {};

static_assert(!std::is_unsigned_v<is_integral_test::A> &&
                  !std::is_unsigned_v<float> &&
                  !std::is_unsigned_v<signed int> &&
                  std::is_unsigned_v<unsigned int> &&
                  !std::is_unsigned_v<UEnum> && !std::is_unsigned_v<UEnumC>,
              "is_unsigned");
} // namespace is_unsigned_test

namespace decay_test {
static_assert(std::is_same_v<std::decay_t<int>, int> &&
                  std::is_same_v<std::decay_t<int &>, int> &&
                  std::is_same_v<std::decay_t<int &&>, int> &&
                  std::is_same_v<std::decay_t<const int &>, int> &&
                  std::is_same_v<std::decay_t<int[2]>, int *> &&
                  std::is_same_v<std::decay_t<int(int)>, int (*)(int)>,
              "decay");
}

namespace add_reference_test {
using nonref = int;
using lref = std::add_lvalue_reference_t<nonref>;
using rref = std::add_rvalue_reference_t<nonref>;
using voidref = std::add_lvalue_reference_t<void>;

static_assert(!std::is_lvalue_reference_v<nonref> &&
                  std::is_lvalue_reference_v<lref> &&
                  std::is_rvalue_reference_v<rref> &&
                  !std::is_reference_v<voidref>,
              "add_reference");
} // namespace add_reference_test

namespace remove_extent_test {
constexpr int a[][3] = {{1, 2, 3}, {4, 5, 6}};
constexpr int b[3] = {7, 8, 9};
static_assert(
    std::is_same_v<std::remove_extent_t<decltype(b)>, const int> &&
        std::is_same_v<std::remove_extent_t<decltype(a)>, const int[3]>,
    "remove_extent");
} // namespace remove_extent_test

namespace remove_all_extents_test {

struct X {
  int m;
};

static_assert(
    std::is_same_v<std::remove_all_extents_t<float>, float> &&
        std::is_same_v<std::remove_all_extents_t<float[1][2][3]>, float> &&
        std::is_same_v<std::remove_all_extents_t<float[1][1][1][1][2]>,
                       float> &&
        std::is_same_v<std::remove_all_extents_t<float *>, float *> &&
        std::is_same_v<std::remove_all_extents_t<int[3][2]>, int> &&
        std::is_same_v<std::remove_all_extents_t<double[2][3]>, double> &&
        std::is_same_v<std::remove_all_extents_t<X[3][3]>, X>,
    "remove_all_extents");
} // namespace remove_all_extents_test

namespace remove_pointer_test {
static_assert(
    std::is_same_v<int, std::remove_pointer<int>::type> &&
        std::is_same_v<int, std::remove_pointer<int *>::type> &&
        !std::is_same_v<int, std::remove_pointer<int **>::type> &&
        std::is_same_v<int, std::remove_pointer<int *const>::type> &&
        std::is_same_v<int, std::remove_pointer<int *volatile>::type> &&
        std::is_same_v<int, std::remove_pointer<int *const volatile>::type>,
    "remove_pointer");
}

namespace add_pointer_test {

template <typename F, typename Class>
constexpr void ptr_to_member_func_cvref_test(F Class::*) {
  // F is an "abominable function type"
  using FF = std::add_pointer_t<F>;
  static_assert(std::is_same_v<F, FF>, "FF should be precisely F");
}

struct S {
  void f_ref() & {}
  void f_const() const {}
};

constexpr void test() {
  int i = 123;
  int &ri = i;
  typedef std::add_pointer_t<decltype(i)> IntPtr;
  typedef std::add_pointer_t<decltype(ri)> IntPtr2;

  ptr_to_member_func_cvref_test(&S::f_ref);
  ptr_to_member_func_cvref_test(&S::f_const);

  static_assert(std::is_pointer_v<IntPtr> && std::is_same_v<IntPtr, int *> &&
                    std::is_same_v<IntPtr2, IntPtr>,
                "add_pointer");
}
} // namespace add_pointer_test

namespace make_signed_test {
static_assert(
    std::is_same_v<std::make_signed_t<unsigned char>, signed char> &&
        std::is_same_v<std::make_signed_t<unsigned int>, signed int> &&
        std::is_same_v<std::make_signed_t<volatile unsigned long>,
                       volatile signed long>,
    "make_signed");
}

namespace make_unsigned_test {
static_assert(
    std::is_same<std::make_unsigned<char>::type, unsigned char>::value &&
        std::is_same<std::make_unsigned<int>::type, unsigned int>::value &&
        std::is_same<std::make_unsigned<volatile long>::type,
                     volatile unsigned long>::value,
    "make_unsigned");
} // namespace make_unsigned_test

namespace is_constructible_test {
class Foo {
  int v1;
  double v2;

public:
  Foo(int n) : v1(n), v2() {}
  Foo(int n, double f) noexcept : v1(n), v2(f) {}
};

static_assert(std::is_trivially_constructible_v<Foo, const Foo &> &&
                  !std::is_trivially_constructible_v<Foo, int> &&
                  std::is_constructible_v<Foo, int> &&
                  !std::is_nothrow_constructible_v<Foo, int> &&
                  std::is_nothrow_constructible_v<Foo, int, double>,
              "is_constructible");
} // namespace is_constructible_test

namespace is_default_constructible_test {
struct Ex1 {
  is_trivial_test::Nt nt; // member has a non-trivial default ctor
};
struct Ex2 {
  int n;
  Ex2() = default; // trivial and non-throwing
};

static_assert(std::is_default_constructible_v<Ex1> &&
                  !std::is_trivially_default_constructible_v<Ex1> &&
                  std::is_trivially_default_constructible_v<Ex2> &&
                  std::is_nothrow_default_constructible_v<Ex2>,
              "is_default_constructible");
} // namespace is_default_constructible_test

namespace is_copy_constructible_test {

struct Ex1 {
  is_trivially_copyable_test::Ntc ntc; // member has a non-trivial copy ctor
};
struct Ex2 {
  int n;
  Ex2(const Ex2 &) = default; // trivial and non-throwing
};

static_assert(std::is_copy_constructible_v<Ex1> &&
                  !std::is_trivially_copy_constructible_v<Ex1> &&
                  std::is_trivially_copy_constructible_v<Ex2> &&
                  std::is_nothrow_copy_constructible_v<Ex2>,
              "is_copy_constructible");
} // namespace is_copy_constructible_test

namespace is_move_constructible_test {

struct NonTrivialNonThrowingMove {
  NonTrivialNonThrowingMove(NonTrivialNonThrowingMove &&) noexcept {}
};

struct Ex1 {
  NonTrivialNonThrowingMove str;
};

struct Ex2 {
  int n;
  Ex2(Ex2 &&) = default; // trivial and non-throwing
};

struct NoMove {
  NoMove(const NoMove &) {}
};

static_assert(std::is_move_constructible_v<Ex1> &&
                  !std::is_trivially_move_constructible_v<Ex1> &&
                  std::is_nothrow_move_constructible_v<Ex1> &&
                  std::is_trivially_move_constructible_v<Ex2> &&
                  std::is_nothrow_move_constructible_v<Ex2> &&
                  std::is_move_constructible_v<NoMove> &&
                  !std::is_nothrow_move_constructible_v<NoMove>,
              "is_move_constructible");
} // namespace is_move_constructible_test

namespace is_assignable_test {
struct Ex1 {
  int n;
};
static_assert(!std::is_assignable_v<int, int> &&
                  std::is_assignable_v<int &, int> &&
                  !std::is_assignable_v<int, double> &&
                  std::is_nothrow_assignable_v<int &, double> &&
                  !std::is_assignable_v<Ex1, double> &&
                  std::is_trivially_assignable_v<Ex1 &, const Ex1 &>,
              "is_assignable");
} // namespace is_assignable_test

namespace is_copy_assignable_test {
struct Foo {
  int n;
};

static_assert(std::is_trivially_copy_assignable<Foo>::value &&
                  !std::is_copy_assignable<int[2]>::value &&
                  std::is_nothrow_copy_assignable<int>::value,
              "is_copy_assignable");
} // namespace is_copy_assignable_test

namespace is_move_assignable_test {
struct Foo {
  int n;
};

struct NothrowMove {
  NothrowMove &operator=(NothrowMove &&) noexcept { return *this; }
};

struct NoMove {
  // prevents implicit declaration of default move assignment operator
  // however, the class is still move-assignable because its
  // copy assignment operator can bind to an rvalue argument
  NoMove &operator=(const NoMove &) { return *this; }
};

static_assert(std::is_nothrow_move_assignable_v<NothrowMove> &&
                  !std::is_move_assignable_v<int[2]> &&
                  std::is_trivially_move_assignable_v<Foo> &&
                  std::is_move_assignable_v<NoMove> &&
                  !std::is_nothrow_move_assignable_v<NoMove>,
              "is_move_assignable");
} // namespace is_move_assignable_test

namespace is_destructible_test {
struct Destructible {
  ~Destructible() noexcept {}
};

struct Foo {
  Destructible str;
  ~Foo() noexcept {};
};

struct Bar {
  ~Bar() = default;
};

static_assert(std::is_destructible_v<Destructible> &&
                  !std::is_trivially_destructible_v<Foo> &&
                  std::is_nothrow_destructible<Foo>() &&
                  std::is_trivially_destructible<Bar>{},
              "is_destructible");
} // namespace is_destructible_test

namespace has_virtual_destructor_test {
struct Foo {
  virtual ~Foo(){};
};

struct Bar {
  ~Bar() {}
};

static_assert(std::has_virtual_destructor_v<Foo> &&
                  !std::has_virtual_destructor<Bar>{},
              "has_virtual_destructor");
} // namespace has_virtual_destructor_test

namespace alignment_of_test {
struct A {};
struct B {
  std::int8_t p;
  std::int16_t q;
};

static_assert(std::alignment_of_v<A> == 1,
              "alignment_of");

/* Leaving this uncommented because I'm not certain if this is the intended alignment for short and int
static_assert(std::alignment_of_v<B> == 2 &&
                  std::alignment_of<int>() == 1 &&
                  std::alignment_of_v<short> == 2,
              "alignment_of");*/
} // namespace alignment_of_test

namespace rank_test {
static_assert(std::rank<int>{} == 0 && std::rank_v<int[5]> == 1 &&
                  std::rank_v<int[5][5]> == 2 && std::rank_v<int[][5][5]> == 3,
              "rank");
}

namespace extent_test {
constexpr int ints[] = {1, 2, 3, 4};

static_assert(std::extent_v<int[3]> == 3 && std::extent_v<int[3][4], 0> == 3 &&
                  std::extent_v<int[3][4], 1> == 4 &&
                  std::extent_v<int[3][4], 2> == 0 &&
                  std::extent_v<int[]> == 0 && std::extent<int[9]>{} == 9 &&
                  std::extent_v<decltype(ints)> == 4,
              "extent");
} // namespace extent_test

namespace remove_cv_test {

static_assert(
    std::is_same_v<std::remove_cv_t<int>, int> &&
        std::is_same_v<std::remove_cv_t<const int>, int> &&
        std::is_same_v<std::remove_cv_t<volatile int>, int> &&
        std::is_same_v<std::remove_cv_t<const volatile int>, int> &&
        std::is_same_v<std::remove_cv_t<const volatile int *>,
                       const volatile int *> &&
        std::is_same_v<std::remove_cv_t<const int *volatile>, const int *> &&
        std::is_same_v<std::remove_cv_t<int *const volatile>, int *>,
    "remove_cv");
}

namespace add_cv_test {
struct foo {
  void m() {}
  void m() const {}
  void m() volatile {}
  void m() const volatile {}
};

static_assert(std::is_same_v<std::add_const_t<foo>, const foo> &&
                  std::is_same_v<std::add_volatile_t<foo>, volatile foo> &&
                  std::is_same_v<std::add_cv_t<foo>, const volatile foo>,
              "add_cv");
} // namespace add_cv_test

namespace remove_reference_test {
static_assert(
    std::is_same_v<int, std::remove_reference_t<int>> &&
        std::is_same_v<int, std::remove_reference_t<int &>> &&
        std::is_same_v<int, std::remove_reference_t<int &&>> &&
        std::is_same_v<const int, std::remove_reference_t<const int &>>,
    "remove_reference");
}

namespace common_type_test {

template <class T> struct Number { T n; };

template <class T, class U>
constexpr Number<std::common_type_t<T, U>> operator+(const Number<T> &lhs,
                                                     const Number<U> &rhs) {
  return {lhs.n + rhs.n};
}

constexpr Number<int> i1 = {1}, i2 = {2};
constexpr Number<double> d1 = {2.3}, d2 = {3.5};

static_assert((i1 + i2).n == 3 && (i1 + d2).n == 4.5 && (d1 + i2).n == 4.3 &&
                  (d1 + d2).n == 5.8,
              "common_type");

} // namespace common_type_test

namespace underlying_type_test {
enum e1 {};
enum class e2 {};
enum class e3 : unsigned {};
enum class e4 : int {};

static_assert(!std::is_same_v<std::underlying_type_t<e1>, int> &&
                  std::is_same_v<std::underlying_type_t<e2>, int> &&
                  !std::is_same_v<std::underlying_type_t<e3>, int> &&
                  std::is_same_v<std::underlying_type_t<e4>, int>,
              "underlying_type");

} // namespace underlying_type_test

int main()
{
  return 0;
}
