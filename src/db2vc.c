/*
 * db2vc.c
 *
 *  Created on: 1 Jun 2017
 *      Author: mherring
 */

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib-2.0/glib.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus.h>
#include "dbcan.h"

DBusHandlerResult signal_filter(DBusConnection *conn, DBusMessage *msg, void *user_data) {
  DBusMessageIter args;
  DBusMessageIter ents;
  DBusMessageIter eles;
  DBusMessageIter vari;
  const char* path;
  int ival;
  double fval;

  if (dbus_message_is_signal(msg, "com.victronenergy.BusItem", "PropertiesChanged")) {
    if (dbus_message_iter_init(msg, &args)) {
      path = dbus_message_get_path(msg);
      if ((dbus_message_iter_get_arg_type(&args) == 'a') &&
          (dbus_message_iter_get_element_type(&args) == 'e')) {
        dbus_message_iter_recurse(&args, &ents);
        for (bool isval = false; !isval; dbus_message_iter_next(&ents)) {
          dbus_message_iter_recurse(&ents, &eles);
          char* key;
          dbus_message_iter_get_basic(&eles, &key);
          if (strcmp(key, "Value") == 0) {
            isval = true;
            dbus_message_iter_next(&eles);
            dbus_message_iter_recurse(&eles, &vari);
            switch (dbus_message_iter_get_arg_type(&vari)) {
            case 'd':
              dbus_message_iter_get_basic(&vari, &fval);
              canUpdate(path, true, fval, 0);
              break;
            case 'i':
              dbus_message_iter_get_basic(&vari, &ival);
              canUpdate(path, false, 0, ival);
              break;
            }
          }
          if (!isval && !dbus_message_iter_has_next(&ents)) break;
        }
      }
    }
  }
  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main(int argc, char** argv) {
  DBusConnection* conn;
  DBusError err;

  GMainLoop* loop = g_main_loop_new(NULL, FALSE);
  dbus_error_init(&err);

  conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (dbus_error_is_set(&err)) {
    g_error("Cannot get System BUS connection: %s", err.message);
    dbus_error_free(&err);
    return EXIT_FAILURE;
  }

  dbus_connection_setup_with_g_main(conn, NULL);

  dbus_bus_add_match(conn, "type='signal',interface='com.victronenergy.BusItem'", &err);
  if (dbus_error_is_set(&err)) {
    g_error("Cannot add D-BUS match rule, cause: %s", err.message);
    dbus_error_free(&err);
    return EXIT_FAILURE;
  }

  dbus_connection_add_filter(conn, signal_filter, NULL, NULL);

  if (initCAN())
    g_main_loop_run(loop);
  else
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

