bool Part::isLocalFileValid()
{
    const QString localFile = localFilePath();
    const QFileInfo localFileInfo(localFile);
    const bool creatingNewArchive = arguments().metaData()[QStringLiteral("createNewArchive")] == QLatin1String("true");

    if (localFileInfo.isDir()) {
        displayMsgWidget(KMessageWidget::Error, xi18nc("@info",
                                                       "<filename>%1</filename> is a directory.",
                                                       localFile));
        return false;
    }

    if (creatingNewArchive) {
        if (localFileInfo.exists()) {
            if (!confirmAndDelete(localFile)) {
                displayMsgWidget(KMessageWidget::Error, xi18nc("@info",
                                                               "Could not overwrite <filename>%1</filename>. Check whether you have write permission.",
                                                               localFile));
                return false;
            }
        }

        displayMsgWidget(KMessageWidget::Information, xi18nc("@info", "The archive <filename>%1</filename> will be created as soon as you add a file.", localFile));
    } else {
        if (!localFileInfo.exists()) {
            displayMsgWidget(KMessageWidget::Error, xi18nc("@info", "The archive <filename>%1</filename> was not found.", localFile));
            return false;
        }

        if (!localFileInfo.isReadable()) {
            displayMsgWidget(KMessageWidget::Error, xi18nc("@info", "The archive <filename>%1</filename> could not be loaded, as it was not possible to read from it.", localFile));
            return false;
        }
    }

    return true;
}
