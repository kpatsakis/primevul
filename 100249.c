void RenderView::AcceleratedSurfaceSetIOSurface(gfx::PluginWindowHandle window,
                                                int32 width,
                                                int32 height,
                                                uint64 io_surface_identifier) {
  Send(new ViewHostMsg_AcceleratedSurfaceSetIOSurface(
      routing_id(), window, width, height, io_surface_identifier));
}
