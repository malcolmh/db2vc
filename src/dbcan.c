/*
 * dbcan.c
 *
 *  Created on: 1 Jun 2017
 *      Author: mherring
 */

#include "dbcan.h"

void canUpdate(DBUpdate update) {
  printf("Path: %s, Value: %lf\n", update.isflt ? update.val.fval : (double)update.val.ival);
}
