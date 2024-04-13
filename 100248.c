void AutoFillHelper::FrameWillClose(WebFrame* frame) {
   form_manager_.ResetFrame(frame);
 }
