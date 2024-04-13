void PlatformFontSkia::InitFromDetails(sk_sp<SkTypeface> typeface,
                                       const std::string& font_family,
                                       int font_size_pixels,
                                       int style,
                                       Font::Weight weight,
                                       const FontRenderParams& render_params) {
  TRACE_EVENT0("fonts", "PlatformFontSkia::InitFromDetails");
  DCHECK_GT(font_size_pixels, 0);

  font_family_ = font_family;
  bool success = true;
  typeface_ = typeface ? std::move(typeface)
                       : CreateSkTypeface(style & Font::ITALIC, weight,
                                          &font_family_, &success);

  if (!success) {
    LOG(ERROR) << "Could not find any font: " << font_family << ", "
               << kFallbackFontFamilyName << ". Falling back to the default";

    InitFromPlatformFont(g_default_font.Get().get());
    return;
  }

  font_size_pixels_ = font_size_pixels;
  style_ = style;
  weight_ = weight;
  device_scale_factor_ = GetFontRenderParamsDeviceScaleFactor();
  font_render_params_ = render_params;
}
