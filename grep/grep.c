#include "s21_grep.h"

int main(int argc, char* argv[]) {
  Opt flags = {0};
  if (argc > 2) {
    ParseGrep(argc, argv, &flags);
    if (flags.e == 0 && flags.f == 0) {
      strcat(flags.pattern, argv[optind]);
      optind += 1;
    }
    OpenFiles(argc, argv, &flags);
  }
  return 0;
}

void ParseGrep(int argc, char* argv[], Opt* flags) {
  int currentFlag = 0;
  int counter_e = 0;
  flags->pattern_count = 0;
  struct option longOptions[] = {{0, 0, 0, 0}};
  while ((currentFlag = getopt_long(argc, argv, "e:ivclnhsf:o", longOptions,
                                    NULL)) != -1) {
    switch (currentFlag) {
      case 'e':
        flags->e = 1;
        if (counter_e == 1) {
          strcat(flags->pattern, "|");
        }
        strcat(flags->pattern, optarg);
        flags->pattern_count += strlen(optarg);
        flags->pattern_count++;
        counter_e = 1;
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        GrepArgF(optarg, &counter_e, flags, argv);
        break;
      case 'o':
        flags->o = 1;
        break;
    }
  }
}

void GrepArgF(char* optarg, int* counter_e, Opt* flags, char* argv[]) {
  FILE* fp = NULL;
  char line[4096] = {0};
  int counter_f = 0;

  if ((fp = fopen(optarg, "r")) == NULL) {
    fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], optarg);
    exit(2);
  } else {
    while (fgets(line, 4096, fp) != NULL) {
      if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = 0;
      if (*counter_e > 0) strcat(flags->pattern, "|");
      if (*counter_e == 0 && counter_f > 0) strcat(flags->pattern, "|");
      strcat(flags->pattern, line);
      counter_f++;
    }
    fclose(fp);
  }
}

void OpenFiles(int argc, char* argv[], Opt* flags) {
  FILE* fp = NULL;
  int files_count = argc - optind;

  for (; optind < argc; optind++) {
    if ((fp = fopen(argv[optind], "r")) == NULL) {
      if (flags->s == 0) {
        fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
                argv[optind]);
      }
    } else {
      GrepFile(argv, fp, flags, files_count);
      fclose(fp);
    }
  }
}

void GrepFile(char* argv[], FILE* fp, Opt* flags, int files_count) {
  regex_t regex;
  regmatch_t pmatch[1] = {0};
  size_t nmatch = 1;

  int err = 0;
  char str[4096];
  size_t n_line = 1;
  int lines_count = 0;
  int i_option = 0;

  if (flags->i == 1) i_option = REG_ICASE;
  err = regcomp(&regex, flags->pattern, REG_EXTENDED | i_option);
  while (feof(fp) == 0) {
    if (fgets(str, 4096, fp)) {
      int new_line_o_counter = 1;
      err = regexec(&regex, str, nmatch, pmatch, 0);
      if (flags->v != 0) err = err ? 0 : 1;
      if (err == 0) {
        if (flags->c == 0 && flags->l == 0) {
          if (files_count > 1 && flags->h == 0) {
            printf("%s:", argv[optind]);
          }
          if (flags->n != 0) {
            printf("%lu:", n_line);
          }
          if (flags->o != 0 && flags->v == 0) {
            new_line_o_counter = 0;
            char* ptr = str;
            while (err == 0) {
              if (pmatch[0].rm_eo == pmatch[0].rm_so) break;
              printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                     ptr + pmatch[0].rm_so);
              ptr += pmatch[0].rm_eo;
              err = regexec(&regex, ptr, nmatch, pmatch, REG_NOTBOL);
            }
          }
          if (flags->o == 0 || flags->v == 1) printf("%s", str);
          if (str[strlen(str) - 1] != '\n' && new_line_o_counter != 0) {
            printf("\n");
          }
        }
        lines_count++;
      }
      n_line++;
    }
  }
  if (flags->c == 1) {
    if (files_count > 1 && flags->h == 0) {
      printf("%s:", argv[optind]);
    }
    if (flags->l == 1 && lines_count)
      printf("1\n");
    else
      printf("%d\n", lines_count);
  }
  if (flags->l == 1 && lines_count) {
    printf("%s\n", argv[optind]);
  }
  regfree(&regex);
}
