bool ImageProcessorClient::WaitUntilNumImageProcessed(
    size_t num_processed,
    base::TimeDelta max_wait) {
  base::TimeDelta time_waiting;
  base::AutoLock auto_lock_(output_lock_);
  while (time_waiting < max_wait) {
    if (num_processed_frames_ >= num_processed)
      return true;

    const base::TimeTicks start_time = base::TimeTicks::Now();
    output_cv_.TimedWait(max_wait);
    time_waiting += base::TimeTicks::Now() - start_time;
  }

  return false;
}
