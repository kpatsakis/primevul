update_info_drive_ports (Device *device)
{
  GList *ports;
  GList *l;
  GPtrArray *p;

  ports = daemon_local_find_enclosing_ports (device->priv->daemon, device->priv->native_path);

  p = g_ptr_array_new ();
  for (l = ports; l != NULL; l = l->next)
    {
      Port *port = PORT (l->data);

      g_ptr_array_add (p, (gpointer) port_local_get_object_path (port));
    }
  g_ptr_array_add (p, NULL);
  device_set_drive_ports (device, (GStrv) p->pdata);
  g_ptr_array_unref (p);

  g_list_free (ports);

  return TRUE;
}
