void Location::setHostname(LocalDOMWindow* current_window,
                           LocalDOMWindow* entered_window,
                           const String& hostname,
                           ExceptionState& exception_state) {
  KURL url = GetDocument()->Url();
  url.SetHost(hostname);
  SetLocation(url.GetString(), current_window, entered_window,
              &exception_state);
}
