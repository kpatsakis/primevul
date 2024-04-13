bool Part::openFile()
{
    qCDebug(ARK) << "Attempting to open archive" << localFilePath();

    resetGui();

    if (!isLocalFileValid()) {
        return false;
    }

    const bool creatingNewArchive = arguments().metaData()[QStringLiteral("createNewArchive")] == QLatin1String("true");

    if (creatingNewArchive) {
        createArchive();
    } else {
        loadArchive();
    }

    return true;
}
