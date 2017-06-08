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

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <mnd/mnd.h>

#define SRC_DBCAN_C_
#include "dbcan.h"

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

  printf("Path: %s, Value: ", path);
  if (isflt) {
    printf("d=%lf\n", fval);
  } else {
    printf("i=%d\n", ival);
  }

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
      parg->fval = fval;
    } else {
      parg->type = AINT;
      parg->ival = ival;
    }
    for (int i = 0; parg->pgns[i] != 0; i++) {
      int mpgn = 0;
      Arg** marg = NULL;
      for (int j = 0; pgns[j].pgn != 0; j++) {
        if (pgns[j].pgn == parg->pgns[i]) {
          mpgn = pgns[j].pgn;
          marg = pgns[j].arg;
          break;
        }
      }
      if (marg != NULL) {
        int nargs = 1;
        params[0].typ = M2K_I64;
        params[0].dat.i64 = mpgn;
        for (int j = 0; marg[j] != NULL; j++, nargs++) {
          switch (marg[j]->type) {
          case ASEQ:
            params[nargs].typ = M2K_I64;
            params[nargs].dat.i64 = marg[j]->ival++;
            break;
          case AINT:
            params[nargs].typ = M2K_I64;
            params[nargs].dat.i64 = marg[j]->ival;
            break;
          case AFLT:
            params[nargs].typ = M2K_F64;
            params[nargs].dat.f64 = marg[j]->fval;
            break;
          default:
            break;
          }
        }
        encodeN2000(nargs, params, &message);
        char decode[2000];
        printf("%s\n", decodeN2000(&message, decode));
      }
    }
  }

}
