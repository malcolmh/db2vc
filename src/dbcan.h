/*
 * dbcan.h
 *
 *  Created on: 1 Jun 2017
 *      Author: mherring
 */

#ifndef SRC_DBCAN_H_
#define SRC_DBCAN_H_

#include <stdbool.h>

bool initCAN();
void canUpdate(const char* path, bool isflt, double fval, int ival);

#endif /* SRC_DBCAN_H_ */
#ifdef SRC_DBCAN_C_

typedef enum {ANUL, ASEQ, AFLT, AINT} Atype;

typedef struct pgn PGN;
typedef struct args Args;

struct args {
  Atype type;;
  double fval;
  int ival;
  int pgns[];
};

typedef struct {
  char *name;
  Args* args;
} Path;

struct pgn {
  int pgn;
  Args* Args[];
};

Args AcActiveInL1I = {ANUL, 0, 0, {127744, 0}};
Args AcActiveInL1P = {ANUL, 0, 0, {127744, 0}};
Args AcActiveOutL1I = {ANUL, 0, 0, {127744, 0}};
Args AcActiveOutL1P = {ANUL, 0, 0, {127744, 0}};
Args afix0 = {AINT, 0, 0, {0}};
Args afix1 = {AINT, 0, 1, {0}};
Args aseq = {ASEQ, 0, 0, {0}};

PGN P127744 = {127744, {&aseq, &afix0, &AcActiveInL1I, &AcActiveInL1P, NULL}};

Path paths[] = {{"/Ac/ActiveIn/L1/I", &AcActiveInL1I}, {"/Ac/ActiveOut/L1/I", &AcActiveOutL1I}, {}};

#endif /* SRC_DBCAN_C_ */
