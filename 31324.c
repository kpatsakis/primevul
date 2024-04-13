get_name_owner (DBusConnection     *connection,
		const char         *name,
		GError            **error)
{
  DBusError derror;
  DBusMessage *request, *reply;
  char *base_name;
  
  dbus_error_init (&derror);

  base_name = NULL;
  reply = NULL;

  request = dbus_message_new_method_call (DBUS_SERVICE_DBUS,
					  DBUS_PATH_DBUS,
					  DBUS_INTERFACE_DBUS,
					  "GetNameOwner");
  if (request == NULL)
    g_error ("Out of memory");
  
  if (!dbus_message_append_args (request, 
				 DBUS_TYPE_STRING, &name, 
				 DBUS_TYPE_INVALID))
    g_error ("Out of memory");

  reply =
    dbus_connection_send_with_reply_and_block (connection,
                                               request,
                                               2000, &derror);
  if (reply == NULL)
    goto error;

  if (dbus_set_error_from_message (&derror, reply))
    goto error;

  if (!dbus_message_get_args (reply, &derror, 
			      DBUS_TYPE_STRING, &base_name, 
			      DBUS_TYPE_INVALID))
    goto error;

  base_name = g_strdup (base_name);
  goto out;

 error:
  g_assert (dbus_error_is_set (&derror));
  dbus_set_g_error (error, &derror);
  dbus_error_free (&derror);

 out:
  if (request)
    dbus_message_unref (request);
  if (reply)
    dbus_message_unref (reply);

  return base_name;
}
