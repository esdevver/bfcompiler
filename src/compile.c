
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "containers.h"
#include "instruction.h"

void print_usage(char *program_name) {
  fprintf(stderr,"USAGE: %s FILE\n", program_name);
}

typedef enum STATUS { STATUS_LOOPEND, STATUS_EOF } STATUS;

STATUS read_brainf(FILE *source, vector *instruction_list) {
  char c;
  instruction_unit_count *move;
  instruction_unit_count *add;
  instruction_unit_list *loop;
  instruction_unit *io;
  instruction_unit *last_instruction;
  while ((c = fgetc(source)) != EOF) {
    if (strchr("[]<>-+.,", c) == NULL)
      continue;
    last_instruction = vector_last(instruction_list);
    switch (c) {
    case '[':
      loop = (instruction_unit_list *)malloc(sizeof(instruction_unit_list));
      loop->type = INSTRUCTION_LOOP;
      vector_init(&loop->list);
      vector_reserve(&loop->list, 4);
      vector_push(instruction_list, loop);
      if (read_brainf(source, &loop->list) == STATUS_EOF)
        return STATUS_EOF;
      break;
    case ']':
      return STATUS_LOOPEND;
      break;
    case '<':
      if (last_instruction != NULL &&
          last_instruction->type == INSTRUCTION_MOVE) {
        move = (instruction_unit_count *)last_instruction;
        move->count--;
      } else {
        move = (instruction_unit_count *)malloc(sizeof(instruction_unit_count));
        move->type = INSTRUCTION_MOVE;
        move->count = -1;
        vector_push(instruction_list, move);
      }
      break;
    case '>':
      if (last_instruction != NULL &&
          last_instruction->type == INSTRUCTION_MOVE) {
        move = (instruction_unit_count *)last_instruction;
        move->count++;
      } else {
        move = (instruction_unit_count *)malloc(sizeof(instruction_unit_count));
        move->type = INSTRUCTION_MOVE;
        move->count = 1;
        vector_push(instruction_list, move);
      }
      break;
    case '-':
      if (last_instruction != NULL &&
          last_instruction->type == INSTRUCTION_ADD) {
        add = (instruction_unit_count *)last_instruction;
        add->count--;
        add->count &= 0xFF;
      } else {
        add = (instruction_unit_count *)malloc(sizeof(instruction_unit_count));
        add->type = INSTRUCTION_ADD;
        add->count = 255;
        vector_push(instruction_list, add);
      }
      break;
    case '+':
      if (last_instruction != NULL &&
          last_instruction->type == INSTRUCTION_ADD) {
        add = (instruction_unit_count *)last_instruction;
        add->count++;
        add->count &= 0xFF;
      } else {
        add = (instruction_unit_count *)malloc(sizeof(instruction_unit_count));
        add->type = INSTRUCTION_ADD;
        add->count = 1;
        vector_push(instruction_list, add);
      }
      break;
    case '.':
      io = (instruction_unit *)malloc(sizeof(instruction_unit));
      io->type = INSTRUCTION_OUT;
      vector_push(instruction_list, io);
      break;
    case ',':
      io = (instruction_unit *)malloc(sizeof(instruction_unit));
      io->type = INSTRUCTION_IN;
      vector_push(instruction_list, io);
      break;
    }
  }
  return STATUS_EOF;
}

#ifdef COLOR_OUTPUT
#define PRETTY_INSTRUCTION(COLOR, INSTRUCTION)                                 \
  printf("\x1b[38;5;%sm%s\x1b[0m\n", COLOR, INSTRUCTION)
#define PRETTY_INSTRUCTION_COUNT(COLOR, INSTRUCTION, COUNT)                    \
  printf("\x1b[38;5;%sm%s\x1b[0m \x1b[38;5;166m%d\x1b[0m\n", COLOR,            \
         INSTRUCTION, COUNT)
#else
#define PRETTY_INSTRUCTION(COLOR, INSTRUCTION) printf("%s\n", INSTRUCTION)
#define PRETTY_INSTRUCTION_COUNT(COLOR, INSTRUCTION, COUNT)                    \
  printf("%s %d\n", INSTRUCTION, COUNT)
#endif

void print_syntax(vector *instruction_list, int indent) {
  instruction_unit *instruction;
  instruction_unit_count *move;
  instruction_unit_count *add;
  size_t i;
  int j;
  for (i = 0; i < instruction_list->length; i++) {
    instruction = vector_get(instruction_list, i);
    for (j = 0; j < indent; j++)
      printf("  ");
    switch (instruction->type) {
    case INSTRUCTION_LOOP:
      PRETTY_INSTRUCTION("141", "LOOP");
      print_syntax(&((instruction_unit_list *)instruction)->list, indent + 1);
      break;
    case INSTRUCTION_MOVE:
      move = (instruction_unit_count *)instruction;
      if (move->count == 0)
        PRETTY_INSTRUCTION("246", "NOP");
      else if (move->count > 0) {
        if (move->count == 1)
          PRETTY_INSTRUCTION("167", "RIGHT");
        else
          PRETTY_INSTRUCTION_COUNT("167", "RIGHT", move->count);
      } else {
        if (move->count == -1)
          PRETTY_INSTRUCTION("167", "LEFT");
        else
          PRETTY_INSTRUCTION_COUNT("167", "LEFT", -move->count);
      }
      break;
    case INSTRUCTION_ADD:
      add = (instruction_unit_count *)instruction;
      if (add->count == 0)
        PRETTY_INSTRUCTION("246", "NOP");
      else if (add->count <= 128) {
        PRETTY_INSTRUCTION_COUNT("221", "ADD", add->count);
      } else {
        PRETTY_INSTRUCTION_COUNT("221", "SUB", 256 - add->count);
      }
      break;
    case INSTRUCTION_OUT:
      PRETTY_INSTRUCTION("116", "OUT");
      break;
    case INSTRUCTION_IN:
      PRETTY_INSTRUCTION("116", "IN");
      break;
    }
  }
}

#undef PRETTY_INSTRUCTION
#undef PRETTY_INSTRUCTION_COUNT

int main(int argc, char *argv[]) {

  FILE *source;
  char *error_string;
  vector instruction_list;

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

  vector_init(&instruction_list);
  vector_reserve(&instruction_list, 4);

  read_brainf(source, &instruction_list);

  print_syntax(&instruction_list, 0);

  return 0;
}
