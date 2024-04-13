GLSurfaceOzoneEGL::~GLSurfaceOzoneEGL() {
  Destroy();  // The EGL surface must be destroyed before SurfaceOzone.
}
