void RenderView::OnLoadBlockedPlugins() {
  NotificationService::current()->Notify(NotificationType::SHOULD_LOAD_PLUGINS,
                                         Source<RenderView>(this),
                                         NotificationService::NoDetails());
}
