bool GLSurfaceOzoneSurfacelessSurfaceImpl::Resize(const gfx::Size& size,
                                                  float scale_factor,
                                                  bool has_alpha) {
  if (size == GetSize())
    return true;
  return GLSurfaceOzoneSurfaceless::Resize(size, scale_factor, true) &&
         CreatePixmaps();
}
