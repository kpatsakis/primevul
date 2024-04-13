void RemoteFrame::PointerEventsChanged() {
  if (!cc_layer_ || !is_surface_layer_)
    return;

  static_cast<cc::SurfaceLayer*>(cc_layer_)->SetHasPointerEventsNone(
      IsIgnoredForHitTest());
}
