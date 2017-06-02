/*
 * dbcan.c
 *
 *  Created on: 1 Jun 2017
 *      Author: mherring
 */

#include <stdio.h>
#include <string.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <net/if.h>

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
  printf("Path: %s, Value: ", path);
  if (isflt) {
    printf("%lf\n", fval);
  } else {
    printf("%d\n", ival);
  }
}
