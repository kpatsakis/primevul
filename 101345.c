void HTMLMediaElement::VideoWillBeDrawnToCanvas() const {
  DCHECK(IsHTMLVideoElement());
  UseCounter::Count(GetDocument(), WebFeature::kVideoInCanvas);
  autoplay_policy_->VideoWillBeDrawnToCanvas();
}
