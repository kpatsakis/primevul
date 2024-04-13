    RenderViewImpl* (*create_render_view_impl)(RenderViewImplParams*)) {
  CHECK(!g_create_render_view_impl);
  g_create_render_view_impl = create_render_view_impl;
}
