Document* Document::ContextDocument() const {
  if (context_document_)
    return context_document_;
  if (frame_)
    return const_cast<Document*>(this);
  return nullptr;
}
