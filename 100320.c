gboolean BrowserTitlebar::OnScroll(GtkWidget* widget, GdkEventScroll* event,
                                   BrowserTitlebar* titlebar) {
  TabStripModel* tabstrip_model =
      titlebar->browser_window_->browser()->tabstrip_model();
  int index = tabstrip_model->selected_index();
  if (event->direction == GDK_SCROLL_LEFT ||
      event->direction == GDK_SCROLL_UP) {
    if (index != 0)
      tabstrip_model->SelectPreviousTab();
  } else if (index + 1 < tabstrip_model->count()) {
    tabstrip_model->SelectNextTab();
  }
  return TRUE;
}
