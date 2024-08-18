#include "process.h"
#include "builtins.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Define a default buffer size.
#define HOSTNAME_BUFSIZE 1024
#define BSH_RL_BUFSIZE 1024
#define BSH_RL_FAILURE NULL
#define EXIT_RL_FAILURE 5
#define BSH_TOK_BUFSIZE 64
#define BSH_TOK_DELIM " \t\r\n\a"
#define BSH_TOK_FAILURE NULL
#define EXIT_TOK_FAILURE 6
#define EXIT_LAUNCH_FAILURE 7
#define LAUNCH_SUCCESS 11

void shellKeepAlive(void) {
  // Allocate memory for buffer.
  size_t lineSize = BSH_RL_BUFSIZE;
  char *line = (char *)malloc(lineSize * sizeof(char));

  // Perform cleanup if memory allocation fails.
  if (line == NULL) {
    perror("Shell initialization failed");
    exit(EXIT_FAILURE);
  }

  // Create an identifier to count the characters in the buffer.
  ssize_t charsRead = 0;

  // Create an identifier to store tokens.
  char **args;

  // Create an identifier to store the execution status of programs.
  int status;

  // Create identifiers for username and hostname.
  char *username = getenv("USER");
  char hostname[HOSTNAME_BUFSIZE];

  // Fetch hostname.
  int hostFlag = gethostname(hostname, sizeof(hostname));

  do {
    if (username == NULL || hostFlag != 0) {
      (void)printf("\033[1;32m>\033[0m ");
    } else {
      (void)printf("\033[1;32m%s@%s\033[0m:$ ", username, hostname);
    }

    charsRead = shellReadLine(&line, &lineSize);

    // Perform cleanup if line read fails.
    if (charsRead == -1) {
      if (!feof(stdin)) {
        (void)fprintf(stderr, "Could not read the input\n");

        if (line != NULL) {
          free(line);
        }

        exit(EXIT_RL_FAILURE);
      }
    }

    args = shellTokenize(line);

    // Perform cleanup if tokenization fails.
    if (args == NULL) {
      free(line);
      exit(EXIT_TOK_FAILURE);
    }

    status = shellExecute(args);

    free(line);
    free(args);

    line = NULL;
    lineSize = 0;
  } while (status);

  return;
}

ssize_t shellReadLine(char **buffer, size_t *bufSize) {
  return getline(buffer, bufSize, stdin);
}

char **shellTokenize(char *line) {
  size_t bufsize = BSH_TOK_BUFSIZE, position = 0;
  char **tokens = (char **)malloc(bufsize * sizeof(char *));

  // Perform cleanup if memory allocation fails.
  if (tokens == NULL) {
    perror("Parse error");

    return BSH_TOK_FAILURE;
  }

  // Start tokenization.
  char *token = strtok(line, BSH_TOK_DELIM);

  while (token != NULL) {
    tokens[position++] = token;

    // Increase memory block size if required.
    if (position >= bufsize) {
      bufsize += BSH_TOK_BUFSIZE;
      char **allocGuard = (char **)realloc(tokens, bufsize * sizeof(char *));

      if (allocGuard == NULL) {
        perror("Parse error");
        free(tokens);

        return BSH_TOK_FAILURE;
      }

      tokens = allocGuard;
    }

    token = strtok(NULL, BSH_TOK_DELIM);
  }

  tokens[position] = NULL;

  return tokens;
}

int shellStartProc(char **args) {
  pid_t pid, wpid;
  int status;

  // Create child process.
  pid = fork();

  if (pid == 0) {
    // Child process.
    // Launch new program.
    if (execvp(args[0], args) == -1) {
      perror("Execution error");
      exit(EXIT_LAUNCH_FAILURE);
    }
  } else if (pid < 0) {
    // Error forking.
    perror("Process creation");
  } else {
    // Parent process.
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return LAUNCH_SUCCESS;
}

int shellExecute(char **args) {
  if (args[0] == NULL) {
    // Empty command.
    return 1;
  }

  // Check if the input command is a builtin.
  for (int builtinIndex = 0; builtinIndex < bshNumBuiltins(); ++builtinIndex) {
    if (strcmp(args[0], builtinCmds[builtinIndex]) == 0) {
      return (*builtinFunc[builtinIndex])(args);
    }
  }

  // Start the program.
  return shellStartProc(args);
}
