void RenderView::spellCheck(const WebString& text,
                            int& misspelled_offset,
                            int& misspelled_length) {
  EnsureDocumentTag();

  string16 word(text);
  RenderThread* thread = RenderThread::current();
  if (thread) {
    thread->spellchecker()->SpellCheckWord(
        word.c_str(), word.size(), document_tag_,
        &misspelled_offset, &misspelled_length, NULL);
  }
}
