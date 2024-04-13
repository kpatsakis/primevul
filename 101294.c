void Document::writeln(v8::Isolate* isolate,
                       const Vector<String>& text,
                       ExceptionState& exception_state) {
  if (IsTrustedTypesEnabledForDoc()) {
    DCHECK(origin_trials::TrustedDOMTypesEnabled(this));
    exception_state.ThrowTypeError(
        "This document can only write `TrustedHTML` objects.");
    return;
  }

  if (!AllowedToUseDynamicMarkUpInsertion("writeln", exception_state))
    return;

  StringBuilder builder;
  for (const String& string : text)
    builder.Append(string);
  writeln(builder.ToString(), EnteredDOMWindow(isolate)->document(),
          exception_state);
}
