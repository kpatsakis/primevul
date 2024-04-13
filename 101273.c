void Document::SetIsViewSource(bool is_view_source) {
  is_view_source_ = is_view_source;
  if (!is_view_source_)
    return;
}
