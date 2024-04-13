void PrintRenderFrameHelper::ScriptedPrint(bool user_initiated) {
  if (delegate_->CancelPrerender(render_frame()))
    return;

  blink::WebLocalFrame* web_frame = render_frame()->GetWebFrame();
  if (!IsScriptInitiatedPrintAllowed(web_frame, user_initiated))
    return;

  if (delegate_->OverridePrint(web_frame))
    return;

  if (g_is_preview_enabled) {
#if BUILDFLAG(ENABLE_PRINT_PREVIEW)
    print_preview_context_.InitWithFrame(web_frame);
    RequestPrintPreview(PRINT_PREVIEW_SCRIPTED);
#endif
  } else {
#if BUILDFLAG(ENABLE_BASIC_PRINTING)
    auto weak_this = weak_ptr_factory_.GetWeakPtr();
    web_frame->DispatchBeforePrintEvent();
    if (!weak_this)
      return;
    Print(web_frame, blink::WebNode(), true /* is_scripted? */);
    if (weak_this)
      web_frame->DispatchAfterPrintEvent();
#endif
  }
}
