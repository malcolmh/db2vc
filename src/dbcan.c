/*
 * dbcan.c
 *
 *  Created on: 1 Jun 2017
 *      Author: mherring
 */

#include <stdio.h>
#include "dbcan.h"

void canUpdate(const char* path, bool isflt, double fval, int ival) {
  printf("Path: %s, Value: ", path);
  if (isflt) {
    printf("%lf\n", fval);
  } else {
    printf("%d\n", ival);
  }
}
