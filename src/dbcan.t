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

static PGN AAIL1PI, AOL1PI, AAIL1VF, AOL1VF, D0VI, BStatus;

static Arg AcAInL1I = {ANUL, 0, {&AAIL1PI, NULL}};
static Arg AcAInL1P = {ANUL, 0, {&AAIL1PI, NULL}};
static Arg AcOL1I = {ANUL, 0, {&AOL1PI, NULL}};
static Arg AcOL1P = {ANUL, 0, {&AOL1PI, NULL}};
static Arg AcAInL1F = {ANUL, 0, {&AAIL1VF, NULL}};
static Arg AcAInL1V = {ANUL, 0, {&AAIL1VF, NULL}};
static Arg AcOL1F = {ANUL, 0, {&AOL1VF, NULL}};
static Arg AcOL1V = {ANUL, 0, {&AOL1VF, NULL}};
static Arg Dc0V = {ANUL, 0, {&D0VI, NULL}};
static Arg Dc0I = {ANUL, 0, {&D0VI, NULL}};
static Arg DcBV = {ANUL, 0, {&BStatus, NULL}};
static Arg DcBI = {ANUL, 0, {&BStatus, NULL}};
static Arg afix0 = {AINT, {.ival=0}, {NULL}};
static Arg afix1 = {AINT, {.ival=1}, {NULL}};
static Arg aseq = {ASEQ, 0, {NULL}};
static Arg ana = {ANUL, 0, {NULL}};

static PGN AAIL1PI = {127744, {&aseq, &afix0, &AcAInL1I, &AcAInL1P, NULL}};
static PGN AOL1PI = {127744, {&aseq, &afix1, &AcOL1I, &AcOL1P, NULL}};
static PGN AAIL1VF = {127747, {&aseq, &afix0, &AcAInL1V, &ana, &AcAInL1F, NULL}};
static PGN AOL1VF = {127747, {&aseq, &afix1, &AcOL1V, &ana, &AcOL1F, NULL}};
static PGN D0VI = {127751, {&aseq, &afix0, &Dc0V, &Dc0I, &ana, NULL}};
static PGN BStatus = {127508, {&afix1, &DcBV, &DcBI, &ana, &aseq, NULL}};

static Path paths[] = {{"/Ac/ActiveIn/L1/I", &AcAInL1I}, {"/Ac/Out/L1/I", &AcOL1I},
    {"/Ac/ActiveIn/L1/F", &AcAInL1F}, {"/Ac/Out/L1/F", &AcOL1F},
    {"/Ac/ActiveIn/L1/P", &AcAInL1P}, {"/Ac/Out/L1/P", &AcOL1P},
    {"/Ac/ActiveIn/L1/V", &AcAInL1V}, {"/Ac/Out/L1/V", &AcOL1V},
    {"/Dc/0/Voltage", &Dc0V}, {"/Dc/0/Current", &Dc0I},
    {"/Dc/Battery/Voltage", &DcBV}, {"/Dc/Battery/Current", &DcBI},
    {NULL, NULL}};

#endif /* SRC_DBCAN_T_ */

