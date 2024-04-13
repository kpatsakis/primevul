Comment* Document::createComment(const String& data) {
  return Comment::Create(*this, data);
}
