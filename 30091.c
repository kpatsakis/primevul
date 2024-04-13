set_basic_field (DBusTypeReader       *reader,
                 int                   field,
                 int                   type,
                 const void           *value,
                 const DBusTypeReader *realign_root)
{
  DBusTypeReader sub;
  DBusTypeReader variant;

  _dbus_type_reader_recurse (reader, &sub);

  _dbus_assert (_dbus_type_reader_get_current_type (&sub) == DBUS_TYPE_BYTE);
#ifndef DBUS_DISABLE_ASSERT
 {
   unsigned char v_BYTE;
   _dbus_type_reader_read_basic (&sub, &v_BYTE);
   _dbus_assert (((int) v_BYTE) == field);
 }
#endif

  if (!_dbus_type_reader_next (&sub))
    _dbus_assert_not_reached ("no variant field?");

  _dbus_type_reader_recurse (&sub, &variant);
  _dbus_assert (_dbus_type_reader_get_current_type (&variant) == type);

  if (!_dbus_type_reader_set_basic (&variant, value, realign_root))
    return FALSE;

  return TRUE;
}
