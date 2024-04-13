void Document::writeln(const String& text,
                       Document* entered_document,
                       ExceptionState& exception_state) {
  write(text, entered_document, exception_state);
  if (exception_state.HadException())
    return;
  write("\n", entered_document);
}
