#include "s21_cat.h"

int main(int argc, char *argv[]) {
  Opt flags = ParseCatFlags(argc, argv);
  PrintCat(flags, argc, argv);

  return 0;
}

Opt ParseCatFlags(int argc, char *argv[]) {
  Opt flags = {0, 0, 0, 0, 0, 0};
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {0, 0, 0, 0}};
  int currentFlag = 0;
  while ((currentFlag =
              getopt_long(argc, argv, "bevEnstT", longOptions, NULL)) != -1) {
    switch (currentFlag) {
      case 'b':
        flags.number_not_empty = 1;
        break;
      case 'e':
        flags.not_printable = 1;
        flags.end_of_str = 1;
        break;
      case 'v':
        flags.not_printable = 1;
        break;
      case 'E':
        flags.end_of_str = 1;
        break;
      case 'n':
        flags.number_all = 1;
        break;
      case 's':
        flags.squeeze = 1;
        break;
      case 't':
        flags.tab = 1;
        flags.not_printable = 1;
        break;
      case 'T':
        flags.tab = 1;
        break;
    }
  }
  return flags;
}

void PrintCat(Opt flags, int argc, char *argv[]) {
  int new_line_counter = 0;
  int newline = 1;
  char ch = 0;
  int number_count = 1;
  for (int i = 1; i < argc; ++i) {
    FILE *f_p = fopen(argv[i], "r");
    if (f_p != NULL) {
      while ((ch = fgetc(f_p)) != EOF) {
        if (flags.squeeze && ch == '\n')
          new_line_counter++;
        else
          new_line_counter = -1;
        if (flags.squeeze && new_line_counter > 1) continue;

        if (flags.number_all && !flags.number_not_empty && newline)
          printf("%6d\t", number_count++);
        if (flags.number_not_empty && newline && ch != '\n')
          printf("%6d\t", number_count++);

        if (flags.end_of_str && ch == '\n') printf("$");
        if (ch == '\n')
          newline = 1;
        else
          newline = 0;
        if (flags.tab && ch == '\t') {
          printf("^I");
          continue;
        }
        if (!flags.not_printable) {
          printf("%c", ch);
        }
        if (flags.not_printable && flags.tab && ch == '\t') {
          printf("^I");
          continue;
        }
        if (flags.not_printable && ch < 32) {
          if (ch == 10) {
            printf("\n");
          } else if (ch == 9 && !flags.tab) {
            printf("%c", ch);
          } else if (ch != 9 && ch != 10) {
            printf("^%c", ch + 64);
          }
        } else if (flags.not_printable && ch == 127) {
          printf("^?");
        } else if (flags.not_printable && ch < 127) {
          printf("%c", ch);
        }
      }
      fclose(f_p);
      number_count = 1;
      newline = 1;
      new_line_counter = 0;
    }
  }
}
