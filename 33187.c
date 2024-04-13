void NotificationsEngine::configureNotification(const QString &appName, const QString &eventId)
{
    KNotifyConfigWidget *widget = KNotifyConfigWidget::configure(nullptr, appName);
    if (!eventId.isEmpty()) {
        widget->selectEvent(eventId);
    }
}
