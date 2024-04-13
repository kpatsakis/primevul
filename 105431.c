device_job_cancel (Device *device,
                   DBusGMethodInvocation *context)
{
  uid_t uid;
  const gchar *action_id;

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

  daemon_local_get_uid (device->priv->daemon, &uid, context);

  action_id = NULL;
  if (device->priv->job_initiated_by_uid != uid)
    {
      action_id = "org.freedesktop.udisks.cancel-job-others";
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           action_id,
                           "JobCancel",
                           TRUE,
                           device_job_cancel_authorized_cb,
                           context,
                           0);

 out:
  return TRUE;
}
