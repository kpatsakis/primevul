job_udevadm_settle_child_cb (GPid pid,
                             gint status,
                             gpointer user_data)
{
  Job *job = user_data;
  job_complete (job);
}
