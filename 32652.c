void Part::slotAddFiles(const QStringList& filesToAdd, const Archive::Entry *destination, const QString &relPath)
{
    if (!m_model->archive() || filesToAdd.isEmpty()) {
        return;
    }

    QStringList withChildPaths;
    foreach (const QString& file, filesToAdd) {
        m_jobTempEntries.push_back(new Archive::Entry(Q_NULLPTR, file));
        if (QFileInfo(file).isDir()) {
            withChildPaths << file + QLatin1Char('/');
            QDirIterator it(file, QDir::AllEntries | QDir::Readable | QDir::Hidden | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                QString path = it.next();
                if (it.fileInfo().isDir()) {
                    path += QLatin1Char('/');
                }
                withChildPaths << path;
            }
        } else {
            withChildPaths << file;
        }
    }

    withChildPaths = ReadOnlyArchiveInterface::entryPathsFromDestination(withChildPaths, destination, 0);
    QList<const Archive::Entry*> conflictingEntries;
    bool error = m_model->conflictingEntries(conflictingEntries, withChildPaths, true);

    if (conflictingEntries.count() > 0) {
        QPointer<OverwriteDialog> overwriteDialog = new OverwriteDialog(widget(), conflictingEntries, m_model->entryIcons(), error);
        int ret = overwriteDialog->exec();
        delete overwriteDialog;
        if (ret == QDialog::Rejected) {
            qDeleteAll(m_jobTempEntries);
            m_jobTempEntries.clear();
            return;
        }
    }

    QString globalWorkDir = filesToAdd.first();

    if (!relPath.isEmpty()) {
        globalWorkDir.remove(relPath);
        qCDebug(ARK) << "Adding" << filesToAdd << "to" << relPath;
    } else {
        qCDebug(ARK) << "Adding " << filesToAdd << ((destination == Q_NULLPTR) ? QString() : QStringLiteral("to ") + destination->fullPath());
    }

    if (globalWorkDir.right(1) == QLatin1String("/")) {
        globalWorkDir.chop(1);
    }

    CompressionOptions compOptions = m_compressionOptions;

    globalWorkDir = QFileInfo(globalWorkDir).dir().absolutePath();

    qCDebug(ARK) << "Detected GlobalWorkDir to be " << globalWorkDir;
    compOptions.setGlobalWorkDir(globalWorkDir);

    AddJob *job = m_model->addFiles(m_jobTempEntries, destination, compOptions);
    if (!job) {
        qDeleteAll(m_jobTempEntries);
        m_jobTempEntries.clear();
        return;
    }

    connect(job, &KJob::result,
            this, &Part::slotAddFilesDone);
    registerJob(job);
    job->start();
}
