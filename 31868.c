generate_trivial_inner (DBusMessageDataIter *iter,
                        DBusMessage        **message_p)
{
  DBusMessage *message;

  switch (iter_get_sequence (iter))
    {
    case 0:
      message = dbus_message_new_method_call ("org.freedesktop.TextEditor",
                                              "/foo/bar",
                                              "org.freedesktop.DocumentFactory",
                                              "Create");
      break;
    case 1:
      message = dbus_message_new (DBUS_MESSAGE_TYPE_METHOD_RETURN);
      set_reply_serial (message);
      break;
    case 2:
      message = dbus_message_new_signal ("/foo/bar",
                                         "org.freedesktop.DocumentFactory",
                                         "Created");
      break;
    case 3:
      message = dbus_message_new (DBUS_MESSAGE_TYPE_ERROR);

      if (!dbus_message_set_error_name (message,
                                        "org.freedesktop.TestErrorName"))
        _dbus_assert_not_reached ("oom");
      
      {
        DBusMessageIter iter;
        const char *v_STRING = "This is an error";
        
        dbus_message_iter_init_append (message, &iter);
        if (!dbus_message_iter_append_basic (&iter,
                                             DBUS_TYPE_STRING,
                                             &v_STRING))
          _dbus_assert_not_reached ("oom");
      }
      
      set_reply_serial (message);
      break;
    default:
      return FALSE;
    }
  
  if (message == NULL)
    _dbus_assert_not_reached ("oom");

  *message_p = message;
  
  return TRUE;
}
