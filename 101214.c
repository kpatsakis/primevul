void FrameReference::Reset(blink::WebLocalFrame* frame) {
  if (frame) {
    view_ = frame->view();
    frame_ = frame;
  } else {
    view_ = NULL;
    frame_ = NULL;
  }
}
