int SimulateModifierKeysDown(WebContents* web_contents,
                             bool control,
                             bool shift,
                             bool alt,
                             bool command) {
  int modifiers = 0;

  if (control) {
    modifiers |= blink::WebInputEvent::kControlKey;
    InjectRawKeyEvent(web_contents, blink::WebInputEvent::kRawKeyDown,
                      ui::DomKey::CONTROL, ui::DomCode::CONTROL_LEFT,
                      ui::VKEY_CONTROL, modifiers);
  }
  if (shift) {
    modifiers |= blink::WebInputEvent::kShiftKey;
    InjectRawKeyEvent(web_contents, blink::WebInputEvent::kRawKeyDown,
                      ui::DomKey::SHIFT, ui::DomCode::SHIFT_LEFT,
                      ui::VKEY_SHIFT, modifiers);
  }
  if (alt) {
    modifiers |= blink::WebInputEvent::kAltKey;
    InjectRawKeyEvent(web_contents, blink::WebInputEvent::kRawKeyDown,
                      ui::DomKey::ALT, ui::DomCode::ALT_LEFT, ui::VKEY_MENU,
                      modifiers);
  }
  if (command) {
    modifiers |= blink::WebInputEvent::kMetaKey;
    InjectRawKeyEvent(web_contents, blink::WebInputEvent::kRawKeyDown,
                      ui::DomKey::META, ui::DomCode::META_LEFT,
                      ui::VKEY_COMMAND, modifiers);
  }
  return modifiers;
}
