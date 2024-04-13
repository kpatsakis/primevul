void RenderView::ExecuteCodeImpl(WebFrame* frame,
                                 const ViewMsg_ExecuteCode_Params& params) {
  GURL frame_url = GURL(frame->url());
  if (frame_url.host() == GURL(Extension::ChromeStoreURL()).host()
      && !CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kAllowScriptingGallery)) {
    Send(new ViewMsg_ExecuteCodeFinished(routing_id_, params.request_id, true));
    return;
  }

  std::vector<WebFrame*> frame_vector;
  frame_vector.push_back(frame);
  if (params.all_frames)
    GetAllChildFrames(frame, &frame_vector);

  for (std::vector<WebFrame*>::iterator frame_it = frame_vector.begin();
       frame_it != frame_vector.end(); ++frame_it) {
    WebFrame* frame = *frame_it;
    if (params.is_javascript) {
      if (!UrlMatchesPermissions(frame->url(), params.host_permissions))
        continue;

      std::vector<WebScriptSource> sources;
      sources.push_back(
          WebScriptSource(WebString::fromUTF8(params.code)));
      UserScriptSlave::InsertInitExtensionCode(&sources, params.extension_id);
      frame->executeScriptInIsolatedWorld(
          UserScriptSlave::GetIsolatedWorldId(params.extension_id),
          &sources.front(), sources.size(), EXTENSION_GROUP_CONTENT_SCRIPTS);
    } else {
      frame->insertStyleText(WebString::fromUTF8(params.code), WebString());
    }
  }

  Send(new ViewMsg_ExecuteCodeFinished(routing_id_, params.request_id, true));
}
