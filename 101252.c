void Document::AttachRange(Range* range) {
  DCHECK(!ranges_.Contains(range));
  ranges_.insert(range);
}
