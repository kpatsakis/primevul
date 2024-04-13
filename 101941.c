void Location::replace(LocalDOMWindow* current_window,
                       LocalDOMWindow* entered_window,
                       const USVStringOrTrustedURL& stringOrUrl,
                       ExceptionState& exception_state) {
  String url = GetStringFromTrustedURL(stringOrUrl, current_window->document(),
                                       exception_state);
  if (!exception_state.HadException()) {
    SetLocation(url, current_window, entered_window, &exception_state,
                SetLocationPolicy::kReplaceThisFrame);
  }
}
