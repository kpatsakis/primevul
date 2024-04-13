void* ImageDataNaClBackend::Map() {
  DCHECK(shared_memory_.get());
  if (map_count_++ == 0) {
    shared_memory_->Map(skia_bitmap_.getSize());
    skia_bitmap_.setPixels(shared_memory_->memory());
    skia_bitmap_.setIsOpaque(false);
    skia_canvas_.reset(new SkCanvas(skia_bitmap_));
    return skia_bitmap_.getAddr32(0, 0);
  }
  return shared_memory_->memory();
}
