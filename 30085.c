_dbus_header_set_serial (DBusHeader    *header,
                         dbus_uint32_t  serial)
{
  /* we use this function to set the serial on outgoing
   * messages, and to reset the serial in dbus_message_copy;
   * this assertion should catch a double-set on outgoing.
   */
  _dbus_assert (_dbus_header_get_serial (header) == 0 ||
                serial == 0);

  _dbus_marshal_set_uint32 (&header->data,
                            SERIAL_OFFSET,
			    serial,
                            header->byte_order);
}
