size_t ImageProcessorClient::GetNumOfProcessedImages() const {
  base::AutoLock auto_lock_(output_lock_);
  return num_processed_frames_;
}
