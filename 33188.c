int NotificationsEngine::createNotification(const QString &appName, const QString &appIcon, const QString &summary,
                                            const QString &body, int timeout, const QStringList &actions, const QVariantMap &hints)
{
    Notify(appName, 0, appIcon, summary, body, actions, hints, timeout);
    return m_nextId;
}
