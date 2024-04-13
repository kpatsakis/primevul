device_job_cancel_authorized_cb (Daemon *daemon,
                                 Device *device,
                                 DBusGMethodInvocation *context,
                                 const gchar *action_id,
                                 guint num_user_data,
                                 gpointer *user_data_elements)
{

  if (!device->priv->job_in_progress)
    {
      throw_error (context, ERROR_FAILED, "There is no job to cancel");
      goto out;
    }

  if (!device->priv->job_is_cancellable)
    {
      throw_error (context, ERROR_FAILED, "Job cannot be cancelled");
      goto out;
    }

  job_cancel (device);

  /* TODO: wait returning once the job is actually cancelled? */
  dbus_g_method_return (context);
 out:
  ;
}
