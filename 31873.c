simple_error (void)
{
  DBusMessage *message;
  message =  dbus_message_new (DBUS_MESSAGE_TYPE_ERROR);
  if (message == NULL)
    _dbus_assert_not_reached ("oom");

  if (!dbus_message_set_error_name (message, "foo.bar"))
    _dbus_assert_not_reached ("oom");
  
  set_reply_serial (message);
  
   return message;
 }
