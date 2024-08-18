#include "process.h"
#include <stdlib.h>

int main(void) {
  // Load config files, if any.

  // Run command loop.
  shellKeepAlive();

  // Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}
