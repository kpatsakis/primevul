void RenderView::AddGURLSearchProvider(const GURL& osd_url, bool autodetected) {
  if (!osd_url.is_empty())
    Send(new ViewHostMsg_PageHasOSDD(routing_id_, page_id_, osd_url,
                                     autodetected));
}
