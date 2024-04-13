job_cancel (Device *device)
{
  g_return_if_fail (device->priv->job != NULL);

  device->priv->job->was_cancelled = TRUE;

  /* TODO: maybe wait and user a bigger hammer? (SIGKILL) */
  kill (device->priv->job->pid, SIGTERM);
}
