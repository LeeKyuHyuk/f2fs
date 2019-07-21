#include <readline/readline.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SuperBlock.h"

int commandCheck(const char *text, const char *command);
char **commandCompletion(const char *, int, int);
char *commandGenerator(const char *, int);

char *commandList[] = {"help", "superblock", "groupdescriptor",
                       "exit", "test",       NULL};

int main(int argc, char *argv[]) {
  rl_attempted_completion_function = commandCompletion;

  FILE *file = fopen(argv[1], "r");

  if (argc != 2) {
    printf("Usage: %s <disk file name>\n", argv[0]);
    return EXIT_FAILURE;
  }

  if (file == NULL) {
    printf("Error opening disk file\n");
    return EXIT_FAILURE;
  }

  readSuperBlock(file);

  while (1) {
    char *command = readline("[F2FS] $ ");
    if (command) {
      if (commandCheck("superblock", command)) {
        printSuperBlock(file);
      } else if (commandCheck("exit", command)) {
        return EXIT_SUCCESS;
      } else {
        printf("Command '%s' not found.\n", command);
      }
      free(command);
    }
  }

  return 0;
}

int commandCheck(const char *text, const char *command) {
  return !strncmp(text, command, strlen(text));
}

char **commandCompletion(const char *text, int start, int end) {
  rl_attempted_completion_over = 1;
  return rl_completion_matches(text, commandGenerator);
}

char *commandGenerator(const char *text, int state) {
  static int listIndex, len;
  char *command;

  if (!state) {
    listIndex = 0;
    len = strlen(text);
  }

  while ((command = commandList[listIndex++])) {
    if (strncmp(command, text, len) == 0) {
      return strdup(command);
    }
  }

  return NULL;
}
