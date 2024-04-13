emit_job_changed (Device *device)
{
  drain_pending_changes (device, FALSE);

  if (!device->priv->removed)
    {
      g_print ("**** EMITTING JOB-CHANGED for %s\n", device->priv->native_path);
      g_signal_emit_by_name (device->priv->daemon,
                             "device-job-changed",
                             device->priv->object_path,
                             device->priv->job_in_progress,
                             device->priv->job_id,
                             device->priv->job_initiated_by_uid,
                             device->priv->job_is_cancellable,
                             device->priv->job_percentage,
                             NULL);
      g_signal_emit (device,
                     signals[JOB_CHANGED_SIGNAL],
                     0,
                     device->priv->job_in_progress,
                     device->priv->job_id,
                     device->priv->job_initiated_by_uid,
                     device->priv->job_is_cancellable,
                     device->priv->job_percentage);
    }
}
