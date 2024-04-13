void RenderView::OnClosePage(const ViewMsg_ClosePage_Params& params) {
  WebFrame* main_frame = webview()->mainFrame();
  if (main_frame) {
    const GURL& url = main_frame->url();
    if (url.SchemeIs(chrome::kHttpScheme) ||
        url.SchemeIs(chrome::kHttpsScheme))
      DumpLoadHistograms();
  }
  webview()->dispatchUnloadEvent();

  cross_origin_access_count_ = 0;
  same_origin_access_count_ = 0;

  Send(new ViewHostMsg_ClosePage_ACK(routing_id_, params));
}
