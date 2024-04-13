WebWidget* RenderView::createPopupMenu(const WebPopupMenuInfo& info) {
  RenderWidget* widget = RenderWidget::Create(routing_id_,
                                              render_thread_,
                                              WebKit::WebPopupTypeSelect);
  widget->ConfigureAsExternalPopupMenu(info);
  return widget->webwidget();
}
