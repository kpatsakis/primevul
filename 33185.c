QStringList NotificationsEngine::GetCapabilities()
{
    return QStringList()
        << QStringLiteral("body")
        << QStringLiteral("body-hyperlinks")
        << QStringLiteral("body-markup")
        << QStringLiteral("icon-static")
        << QStringLiteral("actions")
        ;
}
