void HTMLMediaElement::AddPlayedRange(double start, double end) {
  BLINK_MEDIA_LOG << "addPlayedRange(" << (void*)this << ", " << start << ", "
                  << end << ")";
  if (!played_time_ranges_)
    played_time_ranges_ = TimeRanges::Create();
  played_time_ranges_->Add(start, end);
}
