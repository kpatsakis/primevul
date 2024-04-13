void PPB_URLLoader_Impl::didReceiveData(WebURLLoader* loader,
                                        const char* data,
                                        int data_length,
                                        int encoded_data_length) {
  bytes_received_ += data_length;
  UpdateStatus();

  buffer_.insert(buffer_.end(), data, data + data_length);

  DCHECK(request_data_.prefetch_buffer_lower_threshold <
         request_data_.prefetch_buffer_upper_threshold);
  if (!is_streaming_to_file_ &&
      !is_asynchronous_load_suspended_ &&
      (buffer_.size() >= static_cast<size_t>(
          request_data_.prefetch_buffer_upper_threshold))) {
    DVLOG(1) << "Suspending async load - buffer size: " << buffer_.size();
    SetDefersLoading(true);
  }

  if (user_buffer_) {
    RunCallback(FillUserBuffer());
  } else {
    DCHECK(!TrackedCallback::IsPending(pending_callback_));
  }
}
