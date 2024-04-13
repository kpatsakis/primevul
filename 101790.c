void MimeHandlerViewContainer::PostMessageFromValue(
    const base::Value& message) {
  blink::WebLocalFrame* frame = render_frame()->GetWebFrame();
  if (!frame)
    return;

  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(frame->MainWorldScriptContext());
  PostJavaScriptMessage(isolate,
                        content::V8ValueConverter::Create()->ToV8Value(
                            &message, frame->MainWorldScriptContext()));
}
