Element* Document::ElementFromPoint(int x, int y) const {
  if (GetLayoutViewItem().IsNull())
    return nullptr;

  return TreeScope::ElementFromPoint(x, y);
}
