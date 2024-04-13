  bool IsReparentedNode(const AXNode* node) {
    return IsNewNode(node) && IsRemovedNode(node);
  }
