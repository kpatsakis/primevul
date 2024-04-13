void ImageLoader::DispatchDecodeRequestsIfComplete() {
  if (!image_complete_)
    return;

  bool is_active = GetElement()->GetDocument().IsActive();
  if (!is_active || !GetContent() || GetContent()->ErrorOccurred()) {
    RejectPendingDecodes();
    return;
  }

  LocalFrame* frame = GetElement()->GetDocument().GetFrame();
  for (auto& request : decode_requests_) {
    if (request->state() != DecodeRequest::kPendingLoad)
      continue;
    Image* image = GetContent()->GetImage();
    frame->GetChromeClient().RequestDecode(
        frame, image->PaintImageForCurrentFrame(),
        WTF::Bind(&ImageLoader::DecodeRequestFinished,
                  WrapCrossThreadWeakPersistent(this), request->request_id()));
    request->NotifyDecodeDispatched();
  }
}
