void Part::createArchive()
{
    const QString fixedMimeType = arguments().metaData()[QStringLiteral("fixedMimeType")];
    m_model->createEmptyArchive(localFilePath(), fixedMimeType, m_model);

    if (arguments().metaData().contains(QStringLiteral("volumeSize"))) {
        m_model->archive()->setMultiVolume(true);
    }

    const QString password = arguments().metaData()[QStringLiteral("encryptionPassword")];
    if (!password.isEmpty()) {
        m_model->encryptArchive(password,
                                arguments().metaData()[QStringLiteral("encryptHeader")] == QLatin1String("true"));
    }

    updateActions();
    m_view->setDropsEnabled(true);
}
