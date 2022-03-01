
/* Types for AST */

#ifndef BFC_SRC_SYNTAX_H_
#define BFC_SRC_SYNTAX_H_

#include "containers.h"

/* types of syntax */
typedef enum SYNTAX_TYPE {
  SYNTAX_LOOP,
  SYNTAX_MOVE,
  SYNTAX_ADD,
  SYNTAX_OUT,
  SYNTAX_IN
} SYNTAX_TYPE;

/* For types OUT and IN */
typedef struct {
  SYNTAX_TYPE type;
} syntax_unit;

/* For types MOVE and ADD */
typedef struct syntax_count {
  SYNTAX_TYPE type;
  int count;
} syntax_unit_count;

/* For the LOOP type */
typedef struct syntax_list {
  SYNTAX_TYPE type;
  vector list;
} syntax_unit_list;

typedef vector syntax_tree;

#endif
