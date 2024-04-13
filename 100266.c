void RenderView::SendExtensionRequest(const std::string& name,
                                      const ListValue& args,
                                      const GURL& source_url,
                                      int request_id,
                                      bool has_callback) {
  Send(new ViewHostMsg_ExtensionRequest(routing_id_,
                                        name,
                                        args,
                                        source_url,
                                        request_id,
                                        has_callback));
}
