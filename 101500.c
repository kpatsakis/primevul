void PasswordAutofillAgent::PasswordValueGatekeeper::RegisterElement(
    WebInputElement* element) {
  if (was_user_gesture_seen_)
    ShowValue(element);
  else
    elements_.push_back(*element);
}
