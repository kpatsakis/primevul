bool PDFiumEngine::OnMouseDown(const pp::MouseInputEvent& event) {
  pp::MouseInputEvent normalized_event =
      NormalizeMouseEvent(client_->GetPluginInstance(), event);
  if (normalized_event.GetButton() == PP_INPUTEVENT_MOUSEBUTTON_LEFT)
    return OnLeftMouseDown(normalized_event);
  if (normalized_event.GetButton() == PP_INPUTEVENT_MOUSEBUTTON_MIDDLE)
    return OnMiddleMouseDown(normalized_event);
  if (normalized_event.GetButton() == PP_INPUTEVENT_MOUSEBUTTON_RIGHT)
    return OnRightMouseDown(normalized_event);
  return false;
}
