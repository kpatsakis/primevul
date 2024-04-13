void ImageDataNaClBackend::Unmap() {
  if (--map_count_ == 0)
    shared_memory_->Unmap();
}
