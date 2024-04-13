  void SetCurrentSrc(const AtomicString& src) {
    KURL url(src);
    Media()->current_src_ = url;
  }
