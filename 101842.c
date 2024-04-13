void VideoCaptureImpl::SuspendCapture(bool suspend) {
  DCHECK(io_thread_checker_.CalledOnValidThread());
  if (suspend)
    GetVideoCaptureHost()->Pause(device_id_);
  else
    GetVideoCaptureHost()->Resume(device_id_, session_id_, params_);
}
