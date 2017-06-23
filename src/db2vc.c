/*
 ============================================================================
 Name        : db2vc.c
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

#include <stdbool.h>
#include <stdint.h>
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
  DBusError error;
  DBusMessage* msg;
  DBusMessageIter args;
  DBusMessageIter vari;
  DBusPendingCall* pending;
  dbus_uint32_t nad;

  GMainLoop* loop = g_main_loop_new(NULL, FALSE);
  dbus_error_init(&error);

  conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
  if (dbus_error_is_set(&error)) {
    g_error("Cannot get System BUS connection: %s", error.message);
    dbus_error_free(&error);
    return EXIT_FAILURE;
  }

  msg = dbus_message_new_method_call("com.victronenergy.settings",
      "/Settings/Vecan/can0/MainInterface/Nad", "com.victronenergy.BusItem",
      "GetValue");
  if (msg == NULL) {
    fprintf(stderr, "Message Null\n");
    return EXIT_FAILURE;
  }
  if (!dbus_connection_send_with_reply(conn, msg, &pending, -1)) {
    fprintf(stderr, "Out Of Memory\n");
    return EXIT_FAILURE;
  }
  if (pending == NULL) {
    fprintf(stderr, "Pending Call Null\n");
    return EXIT_FAILURE;
  }
  dbus_connection_flush(conn);
  dbus_message_unref(msg);
  dbus_pending_call_block(pending);
  msg = dbus_pending_call_steal_reply(pending);
  if (msg == NULL) {
    fprintf(stderr, "Reply Null\n");
    return EXIT_FAILURE;
  }
  dbus_pending_call_unref(pending);
  if (!dbus_message_iter_init(msg, &args))
    fprintf(stderr, "Message has no arguments\n");
  else if (dbus_message_iter_get_arg_type(&args) != DBUS_TYPE_VARIANT)
    fprintf(stderr, "Argument is not variant\n");
  else {
    dbus_message_iter_recurse(&args, &vari);
    dbus_message_iter_get_basic(&vari, &nad);
  }
  dbus_message_unref(msg);

  dbus_connection_setup_with_g_main(conn, NULL);

  dbus_bus_add_match(conn, "type='signal',interface='com.victronenergy.BusItem'", &error);
  if (dbus_error_is_set(&error)) {
    g_error("Cannot add D-BUS match rule, cause: %s", error.message);
    dbus_error_free(&error);
    return EXIT_FAILURE;
  }

  dbus_connection_add_filter(conn, signal_filter, NULL, NULL);

  if (initCAN(nad)) {
    g_main_loop_run(loop);
  } else {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

