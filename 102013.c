const FontRenderParams& PlatformFontSkia::GetFontRenderParams() {
  TRACE_EVENT0("fonts", "PlatformFontSkia::GetFontRenderParams");
  float current_scale_factor = GetFontRenderParamsDeviceScaleFactor();
  if (current_scale_factor != device_scale_factor_) {
    FontRenderParamsQuery query;
    query.families.push_back(font_family_);
    query.pixel_size = font_size_pixels_;
    query.style = style_;
    query.weight = weight_;
    query.device_scale_factor = current_scale_factor;
    font_render_params_ = gfx::GetFontRenderParams(query, nullptr);
    device_scale_factor_ = current_scale_factor;
  }
  return font_render_params_;
}
