
#ifndef BFC_SRC_INSTRUCTION_H_
#define BFC_SRC_INSTRUCTION_H_

#include "containers.h"
/*
types of instruction
  */
typedef enum INSTRUCTION_TYPE {
  INSTRUCTION_LOOP,
  INSTRUCTION_MOVE,
  INSTRUCTION_ADD,
  INSTRUCTION_OUT,
  INSTRUCTION_IN
} INSTRUCTION_TYPE;

/* For types OUT and IN */
typedef struct {
  INSTRUCTION_TYPE type;
} instruction_unit;

/* For types MOVE and ADD */
typedef struct instruction_count {
  INSTRUCTION_TYPE type;
  int count;
} instruction_unit_count;

/* For the LOOP type */
typedef struct instruction_list {
  INSTRUCTION_TYPE type;
  vector list;
} instruction_unit_list;

#endif
