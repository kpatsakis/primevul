void Instance::DidChangeView(const pp::View& view) {
  pp::Rect view_rect(view.GetRect());
  float device_scale = 1.0f;
  float old_device_scale = device_scale_;
  if (hidpi_enabled_)
    device_scale = view.GetDeviceScale();
  pp::Size view_device_size(view_rect.width() * device_scale,
                            view_rect.height() * device_scale);
  if (view_device_size == plugin_size_ && device_scale == device_scale_)
    return;  // We don't care about the position, only the size.

  image_data_ = pp::ImageData();
  device_scale_ = device_scale;
  plugin_dip_size_ = view_rect.size();
  plugin_size_ = view_device_size;

  paint_manager_.SetSize(view_device_size, device_scale_);

  image_data_ = pp::ImageData(this,
                              PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                              plugin_size_,
                              false);
  if (image_data_.is_null()) {
    DCHECK(plugin_size_.IsEmpty());
    return;
  }

  DisableAutoscroll();

  OnGeometryChanged(zoom_, old_device_scale);
}
