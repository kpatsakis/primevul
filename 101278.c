void Document::UpdateActiveStyle() {
  DCHECK(IsActive());
  DCHECK(IsMainThread());
  TRACE_EVENT0("blink", "Document::updateActiveStyle");
  GetStyleEngine().UpdateActiveStyle();
}
