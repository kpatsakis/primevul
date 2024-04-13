Element* Document::ScrollingElementNoLayout() {
  if (RuntimeEnabledFeatures::ScrollTopLeftInteropEnabled()) {
    if (InQuirksMode()) {
      DCHECK(lifecycle_.GetState() >= DocumentLifecycle::kStyleClean);
      HTMLBodyElement* body = FirstBodyElement();
      if (body && body->GetLayoutObject() &&
          body->GetLayoutObject()->HasOverflowClip())
        return nullptr;

      return body;
    }

    return documentElement();
  }

  return body();
}
