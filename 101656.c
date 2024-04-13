void AppCacheGroup::CancelUpdate() {
  if (update_job_) {
    delete update_job_;
    DCHECK(!update_job_);
    DCHECK_EQ(IDLE, update_status_);
  }
}
