NotificationsEngine::~NotificationsEngine()
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.unregisterService( QStringLiteral("org.freedesktop.Notifications") );
}
