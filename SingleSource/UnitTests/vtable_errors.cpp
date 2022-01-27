#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <exception>

// This test requires __cxa_pure_virtual and __cxa_deleted_virtual to be
// defined in order to link successfully.

// At runtime it will invoke __cxa_pure_virtual();

class Base {
public:
  Base() { methodA(); }

  virtual ~Base();
  virtual const char *methodA() = 0;
  virtual void methodDeleted() = delete;

  static Base &polyFactory();
};

int main() {

  std::set_terminate([]() {
    exit(1);
  });

  puts(Base::polyFactory().methodA());

  return 0;
}

// Key function for class "Base".
Base::~Base() {}

class SingleInheritance : public Base {
public:
  virtual ~SingleInheritance();
  virtual const char *methodA();
};

SingleInheritance::~SingleInheritance() {}

const char * SingleInheritance::methodA() { return ""; }

Base &Base::polyFactory() {
  static SingleInheritance si;
  return si;
}
