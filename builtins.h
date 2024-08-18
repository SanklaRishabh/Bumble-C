#ifndef _BUILTINS_H
#define _BUILTINS_H

extern char *builtinCmds[];
extern int (*builtinFunc[])(char **);
int bshNumBuiltins(void);
int bshCd(char **);
int bshHelp(char **);
int bshExit(char **);

#endif
