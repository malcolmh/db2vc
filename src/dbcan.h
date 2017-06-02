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

#ifdef SRC_DBCAN_C_

typedef enum {ANUL, ASEQ, AFLT, AINT} Atype;

typedef struct pgn PGN;

typedef struct {
  Atype type;;
  double fval;
  int ival;
  PGN* pgns[];
} Args;

typedef struct {
  char *name;
  Args* args;
} Path;

struct pgn {
  int pgn;
  Args* Args[];
};

PGN P127744;

Args AcActiveInL1I = {ANUL, 0, 0, {&P127744, NULL}};
Args AcActiveInL1P = {ANUL, 0, 0, {&P127744, NULL}};
Args AcActiveOutL1I = {ANUL, 0, 0, {&P127744, NULL}};
Args AcActiveOutL1P = {ANUL, 0, 0, {&P127744, NULL}};
Args afix0 = {AINT, 0, 0, {NULL}};
Args afix1 = {AINT, 0, 1, {NULL}};
Args aseq = {ASEQ, 0, 0, {NULL}};

PGN P127744 = {127744, {&aseq, &afix0, &AcActiveInL1I, &AcActiveInL1P, NULL}};

Path paths[] = {{"/Ac/ActiveIn/L1/I", &AcActiveInL1I}, {"/Ac/ActiveOut/L1/I", &AcActiveOutL1I}, {}};

#endif /* SRC_DBCAN_H_ */
#endif /* SRC_DBCAN_C_ */
