void WebContentsImpl::Observe(int type,
                              const NotificationSource& source,
                              const NotificationDetails& details) {
  switch (type) {
    case NOTIFICATION_RENDER_WIDGET_HOST_DESTROYED: {
      RenderWidgetHost* host = Source<RenderWidgetHost>(source).ptr();
      for (PendingWidgetViews::iterator i = pending_widget_views_.begin();
           i != pending_widget_views_.end(); ++i) {
        if (host->GetView() == i->second) {
          pending_widget_views_.erase(i);
          break;
        }
      }
      break;
    }
    default:
      NOTREACHED();
  }
}
