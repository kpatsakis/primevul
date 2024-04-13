generate_many_bodies_inner (DBusMessageDataIter *iter,
                            DBusMessage        **message_p)
{
  DBusMessage *message;
  DBusString signature;
  DBusString body;

  /* Keeping this small makes things go faster */
  message = dbus_message_new_method_call ("o.z.F",
                                          "/",
                                          "o.z.B",
                                          "Nah");
  if (message == NULL)
    _dbus_assert_not_reached ("oom");

  set_reply_serial (message);

  if (!_dbus_string_init (&signature) || !_dbus_string_init (&body))
    _dbus_assert_not_reached ("oom");
  
  if (dbus_internal_do_not_use_generate_bodies (iter_get_sequence (iter),
                                                message->byte_order,
                                                &signature, &body))
    {
      const char *v_SIGNATURE;

      v_SIGNATURE = _dbus_string_get_const_data (&signature);
      if (!_dbus_header_set_field_basic (&message->header,
                                         DBUS_HEADER_FIELD_SIGNATURE,
                                         DBUS_TYPE_SIGNATURE,
                                         &v_SIGNATURE))
        _dbus_assert_not_reached ("oom");

      if (!_dbus_string_move (&body, 0, &message->body, 0))
        _dbus_assert_not_reached ("oom");

      _dbus_marshal_set_uint32 (&message->header.data, BODY_LENGTH_OFFSET,
                                _dbus_string_get_length (&message->body),
                                message->byte_order);
      
      *message_p = message;
    }
  else
    {
      dbus_message_unref (message);
      *message_p = NULL;
    }
  
  _dbus_string_free (&signature);
  _dbus_string_free (&body);

  return *message_p != NULL;
}
