job_complete (Job *job)
{
  if (job->device != NULL && job->job_id != NULL)
    {
      job->device->priv->job_in_progress = FALSE;
      g_free (job->device->priv->job_id);
      job->device->priv->job_id = NULL;
      job->device->priv->job_initiated_by_uid = 0;
      job->device->priv->job_is_cancellable = FALSE;
      job->device->priv->job_percentage = -1.0;

      job->device->priv->job = NULL;
    }

  job->job_completed_func (job->context,
                           job->device,
                           job->was_cancelled,
                           job->status,
                           job->error_string->str,
                           job->stdout_string->str,
                           job->user_data);

  if (job->device != NULL && job->job_id != NULL)
    {
      emit_job_changed (job->device);
    }

  job_free (job);
}
