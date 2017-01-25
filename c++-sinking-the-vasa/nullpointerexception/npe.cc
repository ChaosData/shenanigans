#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <memory>
#include <exception>

std::vector<std::unique_ptr<sigjmp_buf>> jmps;

class NullPointerException : public std::exception { };

static void signal_handler(int) {
  if (jmps.size() > 0) {
    auto& u_jmp = jmps.back();
    sigjmp_buf * jmp = u_jmp.release();
    jmps.pop_back();
    sigjmp_buf cpy;
    memcpy(&cpy, jmp, sizeof(sigjmp_buf));
    free(reinterpret_cast<void*>(jmp));
    siglongjmp(cpy, 1);
  } else {
    signal(SIGSEGV, SIG_DFL);
  }
}


/*
 * note: why this needs to be a macro and not even an inline is a mystery.
 *       one hint though, libstdc++'s internal exception handling is garbage.
 *       libc++ on the other hand works with a single global jmp_buf across
 *       multiple layers of try/catches, and the catch_null equivalent doesn't
 *       even need to be within a try block.
**/
#define catch_null() {\
  sigjmp_buf* p_jmp = reinterpret_cast<sigjmp_buf*>(calloc(1, sizeof(sigjmp_buf)));\
  if (p_jmp != nullptr) {\
    if (sigsetjmp(*p_jmp, 1) != 0) {\
      throw NullPointerException();\
    } else {\
      jmps.emplace_back(std::unique_ptr<sigjmp_buf>(p_jmp));\
    }\
  }\
}

static void setup_signal_handler() {
  signal(SIGSEGV, signal_handler);
}

void nullderef() {
  int* t = 0;
  *t = 1;
}

int main() {
  setup_signal_handler();

  try {
    catch_null();
    try {
      catch_null();
      try {
        catch_null();
        nullderef();
        puts("not printed");
      } catch (NullPointerException const& npe) {
        puts("first null");
      }
      puts("test1");
      nullderef();
      puts("not printed");
    } catch (NullPointerException const& npe) {
      puts("second null");
    }
    puts("test2");
    nullderef();
  } catch (NullPointerException const& npe) {
    puts("third null");
  } catch (std::exception const& e) {
    puts("not printed");
  }
  return 0;
}

