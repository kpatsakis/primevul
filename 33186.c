QString NotificationsEngine::GetServerInformation(QString& vendor, QString& version, QString& specVersion)
{
    vendor = QLatin1String("KDE");
    version = QLatin1String("2.0"); // FIXME
    specVersion = QLatin1String("1.1");
    return QStringLiteral("Plasma");
}
