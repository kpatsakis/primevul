_dbus_header_cache_one (DBusHeader     *header,
                        int             field_code,
                        DBusTypeReader *variant_reader)
{
  header->fields[field_code].value_pos =
    _dbus_type_reader_get_value_pos (variant_reader);

#if 0
  _dbus_verbose ("cached value_pos %d for field %d\n",
                 header->fields[field_code].value_pos, field_code)
#endif
}
