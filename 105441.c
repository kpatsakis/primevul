device_linux_md_remove_component_authorized_cb (Daemon *daemon,
                                                Device *device,
                                                DBusGMethodInvocation *context,
                                                const gchar *action_id,
                                                guint num_user_data,
                                                gpointer *user_data_elements)
{
  char *component = user_data_elements[0];
  char **options = user_data_elements[1];
  int n, m;
  char *argv[128];
  GError *error;
  Device *slave;

  slave = daemon_local_find_by_object_path (device->priv->daemon, component);
  if (slave == NULL)
    {
      throw_error (context, ERROR_FAILED, "Component doesn't exist");
      goto out;
    }

  /* check that it really is a component of the md device */
  for (n = 0; n < (int) device->priv->linux_md_slaves->len; n++)
    {
      if (strcmp (component, device->priv->linux_md_slaves->pdata[n]) == 0)
        break;
    }
  if (n == (int) device->priv->linux_md_slaves->len)
    {
      throw_error (context, ERROR_FAILED, "Component isn't part of the running array");
      goto out;
    }

  n = 0;
  argv[n++] = "udisks-helper-linux-md-remove-component";
  argv[n++] = device->priv->device_file;
  argv[n++] = slave->priv->device_file;
  for (m = 0; options[m] != NULL; m++)
    {
      if (n >= (int) sizeof(argv) - 1)
        {
          throw_error (context, ERROR_FAILED, "Too many options");
          goto out;
        }
      /* the helper will validate each option */
      argv[n++] = (char *) options[m];
    }
  argv[n++] = NULL;

  error = NULL;
  if (!job_new (context,
                "LinuxMdRemoveComponent",
                TRUE,
                device,
                argv,
                NULL,
                linux_md_remove_component_completed_cb,
                FALSE,
                remove_component_data_new (context, slave, options),
                (GDestroyNotify) remove_component_data_unref))
    {
      goto out;
    }

 out:
  ;
}
