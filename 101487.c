void ImageProcessorClient::ProcessTask(scoped_refptr<VideoFrame> input_frame,
                                       scoped_refptr<VideoFrame> output_frame) {
  DCHECK_CALLED_ON_VALID_THREAD(image_processor_client_thread_checker_);
  image_processor_->Process(std::move(input_frame), std::move(output_frame),
                            BindToCurrentLoop(base::BindOnce(
                                &ImageProcessorClient::FrameReady,
                                base::Unretained(this), next_frame_index_)));
  next_frame_index_++;
}
