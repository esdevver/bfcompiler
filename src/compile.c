
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "containers.h"
#include "syntax.h"

void print_usage(char *program_name) {
  fprintf(stderr,"USAGE: %s FILE\n", program_name);
}

typedef enum STATUS { STATUS_LOOPEND, STATUS_EOF } STATUS;

STATUS read_brainf(FILE *source, vector *syntax_list) {
  char c;
  syntax_unit_count *move;
  syntax_unit_count *add;
  syntax_unit_list *loop;
  syntax_unit *io;
  syntax_unit *last_syntax;
  STATUS status;
  while ((c = fgetc(source)) != EOF) {
    if (strchr("[]<>-+.,", c) == NULL)
      continue;
    last_syntax = vector_last(syntax_list);
    switch (c) {
    case '[':
      loop = (syntax_unit_list *)malloc(sizeof(syntax_unit_list));
      loop->type = SYNTAX_LOOP;
      vector_init(&loop->list);
      vector_reserve(&loop->list, 4);
      vector_push(syntax_list, loop);
      status = read_brainf(source, &loop->list);
      vector_shink_to_fit(&loop->list);
      if (status == STATUS_EOF)
        return STATUS_EOF;
      break;
    case ']':
      return STATUS_LOOPEND;
      break;
    case '<':
      if (last_syntax != NULL &&
          last_syntax->type == SYNTAX_MOVE) {
        move = (syntax_unit_count *)last_syntax;
        move->count--;
      } else {
        move = (syntax_unit_count *)malloc(sizeof(syntax_unit_count));
        move->type = SYNTAX_MOVE;
        move->count = -1;
        vector_push(syntax_list, move);
      }
      break;
    case '>':
      if (last_syntax != NULL &&
          last_syntax->type == SYNTAX_MOVE) {
        move = (syntax_unit_count *)last_syntax;
        move->count++;
      } else {
        move = (syntax_unit_count *)malloc(sizeof(syntax_unit_count));
        move->type = SYNTAX_MOVE;
        move->count = 1;
        vector_push(syntax_list, move);
      }
      break;
    case '-':
      if (last_syntax != NULL &&
          last_syntax->type == SYNTAX_ADD) {
        add = (syntax_unit_count *)last_syntax;
        add->count--;
        add->count &= 0xFF;
      } else {
        add = (syntax_unit_count *)malloc(sizeof(syntax_unit_count));
        add->type = SYNTAX_ADD;
        add->count = 255;
        vector_push(syntax_list, add);
      }
      break;
    case '+':
      if (last_syntax != NULL &&
          last_syntax->type == SYNTAX_ADD) {
        add = (syntax_unit_count *)last_syntax;
        add->count++;
        add->count &= 0xFF;
      } else {
        add = (syntax_unit_count *)malloc(sizeof(syntax_unit_count));
        add->type = SYNTAX_ADD;
        add->count = 1;
        vector_push(syntax_list, add);
      }
      break;
    case '.':
      io = (syntax_unit *)malloc(sizeof(syntax_unit));
      io->type = SYNTAX_OUT;
      vector_push(syntax_list, io);
      break;
    case ',':
      io = (syntax_unit *)malloc(sizeof(syntax_unit));
      io->type = SYNTAX_IN;
      vector_push(syntax_list, io);
      break;
    }
  }
  return STATUS_EOF;
}

#ifdef COLOR_OUTPUT
#define PRETTY_SYNTAX(COLOR, SYNTAX)                                 \
  printf("\x1b[38;5;%sm%s\x1b[0m\n", COLOR, SYNTAX)
#define PRETTY_SYNTAX_COUNT(COLOR, SYNTAX, COUNT)                    \
  printf("\x1b[38;5;%sm%s\x1b[0m \x1b[38;5;166m%d\x1b[0m\n", COLOR,            \
         SYNTAX, COUNT)
#else
#define PRETTY_SYNTAX(COLOR, SYNTAX) printf("%s\n", SYNTAX)
#define PRETTY_SYNTAX_COUNT(COLOR, SYNTAX, COUNT)                    \
  printf("%s %d\n", SYNTAX, COUNT)
#endif

void print_syntax(vector *syntax_list, int indent) {
  syntax_unit *syntax;
  syntax_unit_count *move;
  syntax_unit_count *add;
  size_t i;
  int j;
  for (i = 0; i < syntax_list->length; i++) {
    syntax = vector_get(syntax_list, i);
    for (j = 0; j < indent; j++)
      printf("  ");
    switch (syntax->type) {
    case SYNTAX_LOOP:
      PRETTY_SYNTAX("141", "LOOP");
      print_syntax(&((syntax_unit_list *)syntax)->list, indent + 1);
      break;
    case SYNTAX_MOVE:
      move = (syntax_unit_count *)syntax;
      if (move->count == 0)
        PRETTY_SYNTAX("246", "NOP");
      else if (move->count > 0) {
        if (move->count == 1)
          PRETTY_SYNTAX("167", "RIGHT");
        else
          PRETTY_SYNTAX_COUNT("167", "RIGHT", move->count);
      } else {
        if (move->count == -1)
          PRETTY_SYNTAX("167", "LEFT");
        else
          PRETTY_SYNTAX_COUNT("167", "LEFT", -move->count);
      }
      break;
    case SYNTAX_ADD:
      add = (syntax_unit_count *)syntax;
      if (add->count == 0)
        PRETTY_SYNTAX("246", "NOP");
      else if (add->count <= 128) {
        PRETTY_SYNTAX_COUNT("221", "ADD", add->count);
      } else {
        PRETTY_SYNTAX_COUNT("221", "SUB", 256 - add->count);
      }
      break;
    case SYNTAX_OUT:
      PRETTY_SYNTAX("116", "OUT");
      break;
    case SYNTAX_IN:
      PRETTY_SYNTAX("116", "IN");
      break;
    }
  }
}

#undef PRETTY_SYNTAX
#undef PRETTY_SYNTAX_COUNT

int main(int argc, char *argv[]) {

  FILE *source;
  char *error_string;
  vector syntax_list;

  if (argc != 2) {
    print_usage(argv[0]);
    return 1;
  }

  source = fopen(argv[1], "r");

  if (source == NULL) {
#define ERROR_MESSAGE "Could not load \""
    error_string = (char *)malloc(strlen(ERROR_MESSAGE) + strlen(argv[1]) + 2);
    strcpy(error_string, ERROR_MESSAGE);
    strcpy(error_string + strlen(ERROR_MESSAGE), argv[1]);
    strcpy(error_string + strlen(ERROR_MESSAGE) + strlen(argv[1]), "\"\0");
    perror(error_string);
#undef ERROR_MESSAGE
  }

  vector_init(&syntax_list);
  vector_reserve(&syntax_list, 4);

  read_brainf(source, &syntax_list);

  vector_shink_to_fit(&syntax_list);

  print_syntax(&syntax_list, 0);

  return 0;
}
