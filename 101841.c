void VideoCaptureImpl::OnDeviceFormatsInUse(
    const VideoCaptureDeviceFormatsCB& callback,
    const media::VideoCaptureFormats& formats_in_use) {
  DCHECK(io_thread_checker_.CalledOnValidThread());
  callback.Run(formats_in_use);
}
