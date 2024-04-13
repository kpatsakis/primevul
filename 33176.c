NotificationsEngine::NotificationsEngine( QObject* parent, const QVariantList& args )
    : Plasma::DataEngine( parent, args ), m_nextId( 1 ), m_alwaysReplaceAppsList({QStringLiteral("Clementine"), QStringLiteral("Spotify"), QStringLiteral("Amarok")})
