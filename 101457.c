std::unique_ptr<SkiaOutputSurface> SkiaOutputSurfaceImpl::Create(
    std::unique_ptr<SkiaOutputSurfaceDependency> deps,
    const RendererSettings& renderer_settings) {
  auto output_surface = std::make_unique<SkiaOutputSurfaceImpl>(
      util::PassKey<SkiaOutputSurfaceImpl>(), std::move(deps),
      renderer_settings);
  if (!output_surface->Initialize())
    output_surface = nullptr;
  return output_surface;
}
