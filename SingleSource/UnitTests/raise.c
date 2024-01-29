#undef NDEBUG
#include <assert.h>
#include <signal.h>

// Originally from the Public Domain C Library (PDCLib).

static volatile sig_atomic_t flag = 0;

static int expected_signal = 0;

static void test_handler(int sig) {
  assert(sig == expected_signal);
  flag = 1;
}

int main(void) {
  /* Could be other than SIG_DFL if you changed the implementation. */
  assert(signal(SIGABRT, SIG_IGN) == SIG_DFL);
  /* Should be ignored. */
  assert(raise(SIGABRT) == 0);
  /* Installing test handler, old handler should be returned */
  assert(signal(SIGABRT, test_handler) == SIG_IGN);
  /* Raising and checking SIGABRT */
  expected_signal = SIGABRT;
  assert(raise(SIGABRT) == 0);
  assert(flag == 1);
  /* Re-installing test handler, should have been reset to default */
  /* Could be other than SIG_DFL if you changed the implementation. */
  assert(signal(SIGABRT, test_handler) == SIG_DFL);
  /* Raising and checking SIGABRT */
  flag = 0;
  assert(raise(SIGABRT) == 0);
  assert(flag == 1);
  /* Installing test handler for different signal... */
  assert(signal(SIGTERM, test_handler) == SIG_DFL);
  /* Raising and checking SIGTERM */
  expected_signal = SIGTERM;
  assert(raise(SIGTERM) == 0);
  assert(flag == 1);
  return 0;
}
