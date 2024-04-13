void WebContentsImpl::ResetOverrideEncoding() {
  encoding_.clear();
  Send(new ViewMsg_ResetPageEncodingToDefault(GetRoutingID()));
}
