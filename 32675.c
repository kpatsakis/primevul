void Part::slotWatchedFileModified(const QString& file)
{
    qCDebug(ARK) << "Watched file modified:" << file;

    QString relPath = file;
    foreach (QTemporaryDir *tmpDir, m_tmpExtractDirList) {
        relPath.remove(tmpDir->path()); //Remove tmpDir.
    }
    relPath = relPath.mid(1); //Remove leading slash.
    if (relPath.contains(QLatin1Char('/'))) {
        relPath = relPath.section(QLatin1Char('/'), 0, -2); //Remove filename.
    } else {
        relPath = QString();
    }

    QString prettyFilename;
    if (relPath.isEmpty()) {
        prettyFilename = file.section(QLatin1Char('/'), -1);
    } else {
        prettyFilename = relPath + QLatin1Char('/') + file.section(QLatin1Char('/'), -1);
    }

    if (KMessageBox::questionYesNo(widget(),
                               xi18n("The file <filename>%1</filename> was modified. Do you want to update the archive?",
                                     prettyFilename),
                               i18nc("@title:window", "File Modified")) == KMessageBox::Yes) {
        QStringList list = QStringList() << file;

        qCDebug(ARK) << "Updating file" << file << "with path" << relPath;
        slotAddFiles(list, Q_NULLPTR, relPath);
    }
    m_fileWatcher->addPath(file);
}
