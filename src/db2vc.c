#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>

void receive() {
  DBusMessage* msg;
  DBusMessageIter args;
  DBusConnection* conn;
  DBusError err;
  int ret;
  char* sigvalue;

  printf("Listening for signals\n");

  dbus_error_init(&err);
  conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Connection Error (%s)\n", err.message);
    dbus_error_free(&err);
  }
  if (conn == NULL) {
    exit(1);
  }
  ret = dbus_bus_request_name(conn, "test.signal.receiver", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Name Error (%s)\n", err.message);
    dbus_error_free(&err);
  }
  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
    exit(1);
  }
  dbus_bus_add_match(conn, "type='signal',interface='com.victronenergy.BusItem'", &err);
  dbus_connection_flush(conn);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Match Error (%s)\n", err.message);
    exit(1);
  }
  printf("Match rule sent\n");

  while (true) {
    dbus_connection_read_write(conn, 0);
    msg = dbus_connection_pop_message(conn);
    if (msg == NULL) {
      sleep(2);
      continue;
    }
    dbus_message_iter_get_arg_type(&args);
    dbus_message_iter_get_basic(&args, &sigvalue);
    printf("Got Signal with value %s\n", sigvalue);
    dbus_message_unref(msg);
  }
  dbus_connection_close(conn);
}

int main(int argc, char** argv) {
  receive();
  return 0;
}
