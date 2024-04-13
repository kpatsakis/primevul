job_local_end (Device *device)
{
  if (!device->priv->job_in_progress || device->priv->job != NULL)
    {
      g_warning ("There is no job running");
      goto out;
    }

  device->priv->job_in_progress = FALSE;
  g_free (device->priv->job_id);
  device->priv->job_id = NULL;
  device->priv->job_initiated_by_uid = 0;
  device->priv->job_is_cancellable = FALSE;
  device->priv->job_percentage = -1.0;
  emit_job_changed (device);
 out:
  ;
}
