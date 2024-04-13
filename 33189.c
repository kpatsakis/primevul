void NotificationsEngine::removeNotification(uint id, uint closeReason)
{
    const QString source = QStringLiteral("notification %1").arg(id);
    if (m_activeNotifications.remove(source) > 0) {
        removeSource(source);
        emit NotificationClosed(id, closeReason);
    }
}
