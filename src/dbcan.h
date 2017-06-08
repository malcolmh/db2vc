/*
 ============================================================================
 Name        : dbcan.h
 Author      : Malcolm Herring
 Version     : 0.1
 Description : Venus DBus to CANbus
 Copyright   : © 2017 Malcolm Herring.
 This file is part of db2vc.

 db2vc is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, version 3 of the License, or
 any later version.

 dbvc is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 A copy of the GNU General Public License can be found here:
 <http://www.gnu.org/licenses/>.
 ============================================================================
 */

#ifndef SRC_DBCAN_H_
#define SRC_DBCAN_H_

#include <stdbool.h>

bool initCAN();
void canUpdate(const char* path, bool isflt, double fval, int ival);

#endif /* SRC_DBCAN_H_ */
#ifdef SRC_DBCAN_C_

typedef enum {ANUL, ASEQ, AINT, AFLT} Atype;

typedef struct {
  Atype type;;
  double fval;
  int ival;
  int pgns[];
} Arg;

typedef struct {
  int pgn;
  Arg** arg;
} PGN;

typedef struct {
  char* name;
  Arg* arg;
} Path;

Arg AcActiveInL1I = {ANUL, 0, 0, {127744, 0}};
Arg AcActiveInL1P = {ANUL, 0, 0, {127744, 0}};
Arg AcActiveOutL1I = {ANUL, 0, 0, {127744, 0}};
Arg AcActiveOutL1P = {ANUL, 0, 0, {127744, 0}};
Arg afix0 = {AINT, 0, 0, {0}};
Arg afix1 = {AINT, 0, 1, {0}};
Arg aseq = {ASEQ, 0, 0, {0}};

Arg* P127744[] = {&aseq, &afix0, &AcActiveInL1I, &AcActiveInL1P, NULL};

PGN pgns[] = {{127744, P127744}, {0, NULL}};

Path paths[] = {{"/Ac/ActiveIn/L1/I", &AcActiveInL1I}, {"/Ac/ActiveOut/L1/I", &AcActiveOutL1I}, {NULL}};

#endif /* SRC_DBCAN_C_ */
