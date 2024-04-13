MimeHandlerViewContainer::MimeHandlerViewContainer(
    content::RenderFrame* render_frame,
    const std::string& mime_type,
    const GURL& original_url)
    : GuestViewContainer(render_frame),
      mime_type_(mime_type),
      original_url_(original_url),
      guest_proxy_routing_id_(-1),
      guest_loaded_(false),
      weak_factory_(this) {
  DCHECK(!mime_type_.empty());
  is_embedded_ = !render_frame->GetWebFrame()->GetDocument().IsPluginDocument();
  g_mime_handler_view_container_map.Get()[render_frame].insert(this);
}
