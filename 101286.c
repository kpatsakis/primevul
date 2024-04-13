Document* Document::open(v8::Isolate* isolate,
                         const AtomicString& type,
                         const AtomicString& replace,
                         ExceptionState& exception_state) {
  if (replace == "replace") {
    UseCounter::Count(Loader(), WebFeature::kDocumentOpenTwoArgsWithReplace);
  }
  open(EnteredDOMWindow(isolate)->document(), exception_state);
  return this;
}
