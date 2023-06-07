#ifndef _SRC_GREP_S21_GREP_H_
#define _SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char pattern[4096];
  int pattern_count;
} Opt;

void ParseGrep(int argc, char* argv[], Opt* flags);
void OpenFiles(int argc, char* argv[], Opt* flags);
void GrepArgF(char* optarg, int* counter_e, Opt* flags, char* argv[]);
void GrepFile(char* argv[], FILE* fp, Opt* flags, int files_count);

#endif