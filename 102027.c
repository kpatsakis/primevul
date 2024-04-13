void WebContentsImpl::FreezePage() {
  SendPageMessage(new PageMsg_FreezePage(MSG_ROUTING_NONE));
}
