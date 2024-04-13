void RenderView::UpdateEncoding(WebFrame* frame,
                                const std::string& encoding_name) {
  if (webview()->mainFrame() == frame &&
      last_encoding_name_ != encoding_name) {
    last_encoding_name_ = encoding_name;

    Send(new ViewHostMsg_UpdateEncoding(routing_id_, last_encoding_name_));
  }
}
