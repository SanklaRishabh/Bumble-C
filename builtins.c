#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// List of builtin commands.
char *builtinCmds[] = {"cd", "help", "exit"};

// List of addresses to the functions of corresponding builtins.
int (*builtinFunc[])(char **) = {&bshCd, &bshHelp, &bshExit};

int bshNumBuiltins() { return sizeof(builtinCmds) / sizeof(char *); }

// Builtin function implementations.
int bshCd(char **args) {
  if (args[1] == NULL) {
    (void)fprintf(stderr, "Expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("cd");
    }
  }

  return 1;
}

int bshHelp(char **args) {
  FILE *version_info = fopen("version_info", "r");

  if (version_info == NULL) {
    perror("Incomplete module");
    (void)fprintf(stderr, "File `version_info` not found.\n");
  } else {
    char *line = NULL;
    size_t lineSize = 0;
    ssize_t chars;

    do {
      chars = getline(&line, &lineSize, version_info);

      if (!feof(version_info)) {
        (void)fputs(line, stdout);
      }
    } while (chars != -1);

    fclose(version_info);
  }

  int numBuiltins = bshNumBuiltins();

  for (int builtinIndex = 0; builtinIndex < numBuiltins; ++builtinIndex) {
    (void)printf(" %s\n", builtinCmds[builtinIndex]);
  }

  (void)printf("Use the 'man' command for information on other programs.\n");

  return 1;
}

int bshExit(char **args) { return EXIT_SUCCESS; }
