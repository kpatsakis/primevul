void RenderViewImpl::didChangeName(WebFrame* frame,
                                   const WebString& name)  {
  if (!renderer_preferences_.report_frame_name_changes)
    return;

  Send(new ViewHostMsg_UpdateFrameName(routing_id_,
                                       frame->identifier(),
                                       !frame->parent(),
                                       UTF16ToUTF8(name)));
}
