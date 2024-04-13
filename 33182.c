bool NotificationsEngine::registerDBusService()
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    bool so = dbus.registerService(QStringLiteral("org.freedesktop.Notifications"));
    if (so) {
        bool ro = dbus.registerObject(QStringLiteral("/org/freedesktop/Notifications"), this);
        if (ro) {
            qDebug() << "Notifications service registered";
            return true;
        } else {
            dbus.unregisterService(QStringLiteral("org.freedesktop.Notifications"));
        }
    }

    qDebug() << "Failed to register Notifications service";
    return false;
}
