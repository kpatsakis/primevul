    native_pixmap_handle(const gfx::GpuMemoryBufferHandle& handle) {
#if defined(OS_LINUX)
  return handle.native_pixmap_handle;
#else
  static gfx::NativePixmapHandle pixmap_handle;
  return pixmap_handle;
#endif
}
