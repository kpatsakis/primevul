double Instance::CalculateZoom(uint32 control_id) const {
  if (control_id == kZoomInButtonId) {
    for (size_t i = 0; i < chrome_page_zoom::kPresetZoomFactorsSize; ++i) {
      double current_zoom = chrome_page_zoom::kPresetZoomFactors[i];
      if (current_zoom - content::kEpsilon > zoom_)
        return current_zoom;
    }
  } else {
    for (size_t i = chrome_page_zoom::kPresetZoomFactorsSize; i > 0; --i) {
      double current_zoom = chrome_page_zoom::kPresetZoomFactors[i - 1];
      if (current_zoom + content::kEpsilon < zoom_)
        return current_zoom;
    }
  }
  return zoom_;
}
