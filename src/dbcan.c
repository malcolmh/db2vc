/*
 ============================================================================
 Name        : dbcan.c
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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <mnd/mnd.h>

#define SRC_DBCAN_C_
#include "dbcan.h"
#include "dbcan.t"

static int can;

bool initCAN() {
  struct sockaddr_can addr;
  struct ifreq ifr;

  can = socket(PF_CAN, SOCK_RAW, CAN_RAW);

  strcpy(ifr.ifr_name, "can0");
  if (ioctl(can, SIOCGIFINDEX, &ifr) < 0) {
    perror("CAN ioctl");
    return false;
  }

  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;

  if (bind(can, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("CAN bind");
    return false;
  }
  return true;
}

void canUpdate(const char* path, bool isflt, double fval, int ival) {
  T_2000 params[100];
  E_2000 message;
  X_2000 frames[32];
  int seq = 0;

  Arg* parg = NULL;
  for (int i = 0; paths[i].name != NULL; i++) {
    if (strcmp(path, paths[i].name) == 0) {
      parg = paths[i].arg;
      break;
    }
  }
  if (parg != NULL) {
    if (isflt) {
      parg->type = AFLT;
      parg->val.fval = fval;
    } else {
      parg->type = AINT;
      parg->val.ival = ival;
    }
    for (int i = 0; parg->pgns[i] != NULL; i++) {
      int npars = 1;
      params[0].typ = M2K_I64;
      params[0].dat.i64 = parg->pgns[i]->pgn;
      Arg** marg = parg->pgns[i]->args;
      for (int j = 0; marg[j] != NULL; j++, npars++) {
        switch (marg[j]->type) {
        case ANUL:
          params[npars].typ = M2K_I64;
          params[npars].dat.i64 = INT64_MAX;
          break;
        case ASEQ:
          params[npars].typ = M2K_I64;
          params[npars].dat.i64 = marg[j]->val.ival++;
          break;
        case AINT:
          params[npars].typ = M2K_I64;
          params[npars].dat.i64 = marg[j]->val.ival;
          break;
        case AFLT:
          params[npars].typ = M2K_F64;
          params[npars].dat.f64 = marg[j]->val.fval;
          break;
        default:
          break;
        }
      }
      encodeN2000(npars, params, &message);
      message.src = 123;
      int nf = framesN2000(&message, seq++, message.src, message.dst, frames);
      for (int j = 0; j < nf; j++) {
        write(can, &frames[j], sizeof(struct can_frame));
      }
    }
  }
}
