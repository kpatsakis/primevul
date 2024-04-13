SkCanvas* ImageDataNaClBackend::GetCanvas() {
  if (!IsMapped())
    return NULL;
  return skia_canvas_.get();
}
