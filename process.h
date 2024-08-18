#ifndef _PROCESS_H
#define _PROCESS_H

#include <stdio.h>

void shellKeepAlive(void);
ssize_t shellReadLine(char **, size_t *);
char **shellTokenize(char *);
int shellStartProc(char **);
int shellExecute(char **);

#endif
