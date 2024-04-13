DOMHighResTimeStamp Performance::timeOrigin() const {
  DCHECK(!time_origin_.is_null());
  return GetUnixAtZeroMonotonic() +
         ConvertTimeTicksToDOMHighResTimeStamp(time_origin_);
}
