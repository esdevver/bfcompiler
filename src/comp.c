
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "containers.h"
#include "instruction.h"

void print_usage(char *program_name) {
  printf("USAGE: %s FILE\n", program_name);
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

  return 0;
}
