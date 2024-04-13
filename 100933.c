void RenderViewImpl::didCreateScriptContext(WebFrame* frame,
                                            v8::Handle<v8::Context> context,
                                            int extension_group,
                                            int world_id) {
  GetContentClient()->renderer()->DidCreateScriptContext(
      frame, context, extension_group, world_id);
}
