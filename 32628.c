bool Part::confirmAndDelete(const QString &targetFile)
{
    QFileInfo targetInfo(targetFile);
    const auto buttonCode = KMessageBox::warningYesNo(widget(),
                                                      xi18nc("@info",
                                                             "The archive <filename>%1</filename> already exists. Do you wish to overwrite it?",
                                                             targetInfo.fileName()),
                                                      i18nc("@title:window", "File Exists"),
                                                      KStandardGuiItem::overwrite(),
                                                      KStandardGuiItem::cancel());

    if (buttonCode != KMessageBox::Yes || !targetInfo.isWritable()) {
        return false;
    }

    qCDebug(ARK) << "Removing file" << targetFile;

    return QFile(targetFile).remove();
}
