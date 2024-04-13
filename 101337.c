bool HTMLMediaElement::HasNativeControls() {
  return ShouldShowControls(RecordMetricsBehavior::kDoRecord);
}
