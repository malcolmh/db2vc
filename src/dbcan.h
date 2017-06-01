/*
 * dbcan.h
 *
 *  Created on: 1 Jun 2017
 *      Author: mherring
 */

#ifndef SRC_DBCAN_H_
#define SRC_DBCAN_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  bool isflt;
  union {
    int64_t ival;
    double fval;
  } val;
  const char* path;
} DBUpdate;

void canUpdate(DBUpdate update);

#endif /* SRC_DBCAN_H_ */
