void OmniboxViewViews::OnGestureEvent(ui::GestureEvent* event) {
  if (!HasFocus() && event->type() == ui::ET_GESTURE_TAP_DOWN) {
    select_all_on_gesture_tap_ = true;

    saved_selection_for_focus_change_ = gfx::Range::InvalidRange();
  }

  views::Textfield::OnGestureEvent(event);

  if (select_all_on_gesture_tap_ && event->type() == ui::ET_GESTURE_TAP) {
    SelectAllForUserGesture();
    if (location_bar_view_)
      location_bar_view_->omnibox_page_action_icon_container_view()
          ->UpdatePageActionIcon(PageActionIconType::kSendTabToSelf);
  }

  if (event->type() == ui::ET_GESTURE_TAP ||
      event->type() == ui::ET_GESTURE_TAP_CANCEL ||
      event->type() == ui::ET_GESTURE_TWO_FINGER_TAP ||
      event->type() == ui::ET_GESTURE_SCROLL_BEGIN ||
      event->type() == ui::ET_GESTURE_PINCH_BEGIN ||
      event->type() == ui::ET_GESTURE_LONG_PRESS ||
      event->type() == ui::ET_GESTURE_LONG_TAP) {
    select_all_on_gesture_tap_ = false;
  }
}
