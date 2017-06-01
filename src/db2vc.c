#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include "dbcan.h"


int main(int argc, char** argv) {
  DBusConnection *conn = NULL;
  DBusError err;
  DBusMessage* msg;
  DBusMessageIter args;
  DBusMessageIter ents;
  DBusMessageIter eles;
  DBusMessageIter vari;
  DBUpdate update;

  dbus_error_init(&err);
  conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "%s", err.message);
    abort();
  }
  dbus_bus_add_match(conn, "type='signal',interface='com.victronenergy.BusItem'", &err);
  dbus_connection_flush(conn);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Match Error (%s)\n", err.message);
    abort();
  }

  while (true) {
    dbus_connection_read_write(conn, 0);
    msg = dbus_connection_pop_message(conn);
    if (msg == NULL) {
      sleep(1);
      continue;
    }
    if (dbus_message_is_signal(msg, "com.victronenergy.BusItem", "PropertiesChanged")) {
      if (dbus_message_iter_init(msg, &args)) {
        update.path = dbus_message_get_path(msg);
        if ((dbus_message_iter_get_arg_type(&args) == 'a') && (dbus_message_iter_get_element_type(&args) == 'e')) {
          dbus_message_iter_recurse(&args, &ents);
          for (bool isval = false; !isval; dbus_message_iter_next(&ents)) {
            dbus_message_iter_recurse(&ents, &eles);
            char* key;
            dbus_message_iter_get_basic(&eles, &key);
            if (strcmp(key, "Value") == 0) {
              isval = true;
              dbus_message_iter_next(&eles);
              dbus_message_iter_recurse(&eles, &vari);
              if (dbus_message_iter_get_arg_type(&vari) == 'd') {
                update.isflt = true;
                dbus_message_iter_get_basic(&vari, &update.val.fval);
              } else if (dbus_message_iter_get_arg_type(&vari) == 'i') {
                update.isflt = false;
                dbus_message_iter_get_basic(&vari, &update.val.ival);
              }
              canUpdate(update);
            }
            if (!isval && !dbus_message_iter_has_next(&ents)) break;
          }
        }
      }
    }
    dbus_message_unref(msg);
  }
  return 0;
}
