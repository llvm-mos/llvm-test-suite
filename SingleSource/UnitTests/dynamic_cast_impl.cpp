#include <typeinfo>
#include <stdio.h>
#include <stdlib.h>

class Base {
public:
  virtual ~Base();

  virtual void method() volatile = 0;

  void print_id() volatile { printf("BASE %u\n", static_cast<unsigned>(m_id)); }

  static unsigned char id_counter;

  unsigned char m_id = id_counter++;
};

unsigned char Base::id_counter = 0;

extern volatile Base *  impl;
 
class Derived : public Base {
public:
  Derived();
  virtual ~Derived();

  virtual void method() volatile { puts("DERIVED"); }
};

class DerivedNext : public Base {
  public:
  DerivedNext();
  virtual ~DerivedNext();

  virtual void method() volatile { puts("DERIVEDNEXT"); }
};

class DiamondTip : public Derived, public DerivedNext {
  public:
  DiamondTip();
  virtual ~DiamondTip();

  virtual void method() volatile { puts("DIAMOND"); }
};

Derived d;
DiamondTip diamond; // has two copies of Base.

class LeftVirtual : public virtual Base {
  virtual void method() volatile { puts("LEFT VIRTUAL"); }
};

class RightVirtual : public virtual Base {
public:
  void side_method() volatile { puts("SIDE CAST OK");}
};

class DiamondVirtual : public LeftVirtual, public RightVirtual {
  virtual void method() volatile { puts("DIAMOND VIRTUAL"); }
};

DiamondVirtual diamondVirt;

template<class From, class To>
To * test_dyn_cast(From * src) {
  const auto dst = dynamic_cast<To *>(src);
  if (!dst) {
    puts("FAILED DYN CAST");
    exit(1);
  }
  return dst;
}

int main() {
  impl = &d;
  
  impl->method();
  impl->print_id();

  test_dyn_cast<volatile Base, volatile Derived>(impl);

  impl = static_cast<volatile Derived *>(&diamond);
  impl->print_id();
  test_dyn_cast<volatile Base, volatile DiamondTip>(impl);
  test_dyn_cast<volatile Base, volatile DerivedNext>(impl);

  static_cast<volatile Base *>(static_cast<volatile DerivedNext *>(&diamond))->print_id();

  test_dyn_cast<DiamondTip, Derived>(&diamond)->method();
  test_dyn_cast<DiamondTip, DerivedNext>(&diamond)->method();

  impl = &diamondVirt;
  static_cast<volatile Base *>(dynamic_cast<volatile LeftVirtual *>(impl))->print_id();
  static_cast<volatile Base *>(dynamic_cast<volatile RightVirtual *>(impl))
      ->print_id();

  {
    auto side_ptr = dynamic_cast<volatile LeftVirtual *>(impl);
    dynamic_cast<volatile RightVirtual *>(side_ptr)->print_id();
  }

  if (dynamic_cast<volatile Derived *>(impl) != nullptr)
    abort();
  
  return 0;
}

volatile Base *impl;

Base::~Base() = default;
Derived::Derived() = default;
Derived::~Derived() = default;
DerivedNext::DerivedNext() = default;
DerivedNext::~DerivedNext() = default;
DiamondTip::DiamondTip() = default;
DiamondTip::~DiamondTip() = default;
