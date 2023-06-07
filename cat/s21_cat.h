#ifndef _SRC_CAT_S21_CAT_H_
#define _SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdio.h>

typedef struct {
  int number_not_empty;
  int end_of_str;
  int number_all;
  int squeeze;
  int tab;
  int not_printable;
} Opt;

void PrintCat(Opt flags, int argc, char *argv[]);
Opt ParseCatFlags(int argc, char *argv[]);

#endif