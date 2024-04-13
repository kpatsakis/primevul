bool RootWindow::DispatchMouseEvent(MouseEvent* event) {
  if (mouse_move_hold_count_) {
    if (event->type() == ui::ET_MOUSE_DRAGGED ||
        (event->flags() & ui::EF_IS_SYNTHESIZED)) {
      held_mouse_move_.reset(new MouseEvent(*event, NULL, NULL));
      return true;
    } else {
      DispatchHeldMouseMove();
    }
  }
  return DispatchMouseEventImpl(event);
}
