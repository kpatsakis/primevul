ImageProcessorClient::ImageProcessorClient(
    std::vector<std::unique_ptr<VideoFrameProcessor>> frame_processors)
    : frame_processors_(std::move(frame_processors)),
      image_processor_client_thread_("ImageProcessorClientThread"),
      output_cv_(&output_lock_),
      num_processed_frames_(0),
      image_processor_error_count_(0) {
  CHECK(image_processor_client_thread_.Start());
  DETACH_FROM_THREAD(image_processor_client_thread_checker_);
}
