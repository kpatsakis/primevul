void PrepareFrameAndViewForPrint::FrameDetached(DetachType detach_type) {
  blink::WebLocalFrame* frame = frame_.GetFrame();
  DCHECK(frame);
  frame->FrameWidget()->Close();
  frame->Close();
  frame_.Reset(nullptr);
}
