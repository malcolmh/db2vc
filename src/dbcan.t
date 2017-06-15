/*
 ============================================================================
 Name        : dbcan.t
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

#ifndef SRC_DBCAN_T_
#define SRC_DBCAN_T_

#include <stdbool.h>
#include <stdint.h>

typedef enum {ANUL, ASEQ, AINT, AFLT} Atype;

typedef struct pgn PGN;

typedef struct {
  Atype type;
  union {
    double fval;
    int ival;
  } val;
  PGN* pgns[];
} Arg;

struct pgn {
  int pgn;
  Arg* args[];
};

typedef struct {
  char* name;
  Arg* arg;
} Path;

static PGN AAIL1PI, AOL1PI, AAIL1VF, AOL1VF;

static Arg AcActiveInL1I = {ANUL, 0, {&AAIL1PI, NULL}};
static Arg AcActiveInL1P = {ANUL, 0, {&AAIL1PI, NULL}};
static Arg AcOutL1I = {ANUL, 0, {&AOL1PI, NULL}};
static Arg AcOutL1P = {ANUL, 0, {&AOL1PI, NULL}};
static Arg AcActiveInL1F = {ANUL, 0, {&AAIL1VF, NULL}};
static Arg AcActiveInL1V = {ANUL, 0, {&AAIL1VF, NULL}};
static Arg AcOutL1F = {ANUL, 0, {&AOL1VF, NULL}};
static Arg AcOutL1V = {ANUL, 0, {&AOL1VF, NULL}};
static Arg afix0 = {AINT, 0, {NULL}};
static Arg afix1 = {AINT, {.ival=1}, {NULL}};
static Arg aseq = {ASEQ, 0, {NULL}};
static Arg ana = {ANUL, 0, {NULL}};

static PGN AAIL1PI = {127744, {&aseq, &afix0, &AcActiveInL1I, &AcActiveInL1P, NULL}};
static PGN AOL1PI = {127744, {&aseq, &afix1, &AcOutL1I, &AcOutL1P, NULL}};
static PGN AAIL1VF = {127747, {&aseq, &afix0, &AcActiveInL1V, &ana, &AcActiveInL1F, NULL}};
static PGN AOL1VF = {127747, {&aseq, &afix1, &AcOutL1V, &ana, &AcOutL1F, NULL}};

static Path paths[] = {{"/Ac/ActiveIn/L1/I", &AcActiveInL1I}, {"/Ac/Out/L1/I", &AcOutL1I},
    {"/Ac/ActiveIn/L1/F", &AcActiveInL1F}, {"/Ac/Out/L1/F", &AcOutL1F},
    {"/Ac/ActiveIn/L1/P", &AcActiveInL1P}, {"/Ac/Out/L1/P", &AcOutL1P},
    {"/Ac/ActiveIn/L1/V", &AcActiveInL1V}, {"/Ac/Out/L1/V", &AcOutL1V},
    {NULL, NULL}};

#endif /* SRC_DBCAN_T_ */

