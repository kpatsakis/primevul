void HTMLMediaElement::SizeChanged() {
  BLINK_MEDIA_LOG << "sizeChanged(" << (void*)this << ")";

  DCHECK(HasVideo());  // "resize" makes no sense in absence of video.
  if (ready_state_ > kHaveNothing && IsHTMLVideoElement())
    ScheduleEvent(event_type_names::kResize);

  if (GetLayoutObject())
    GetLayoutObject()->UpdateFromElement();
}
