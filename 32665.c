void Part::slotPasteFiles(QVector<Kerfuffle::Archive::Entry*> &files, Kerfuffle::Archive::Entry *destination, int entriesWithoutChildren)
{
    if (files.isEmpty()) {
        delete m_destination;
        return;
    }

    QStringList filesPaths = ReadOnlyArchiveInterface::entryFullPaths(files);
    QStringList newPaths = ReadOnlyArchiveInterface::entryPathsFromDestination(filesPaths, destination, entriesWithoutChildren);

    if (ArchiveModel::hasDuplicatedEntries(newPaths)) {
        displayMsgWidget(KMessageWidget::Error, i18n("Entries with the same names can't be pasted to the same destination."));
        delete m_destination;
        return;
    }

    QList<const Archive::Entry*> conflictingEntries;
    bool error = m_model->conflictingEntries(conflictingEntries, newPaths, false);

    if (conflictingEntries.count() != 0) {
        QPointer<OverwriteDialog> overwriteDialog = new OverwriteDialog(widget(), conflictingEntries, m_model->entryIcons(), error);
        int ret = overwriteDialog->exec();
        delete overwriteDialog;
        if (ret == QDialog::Rejected) {
            delete m_destination;
            return;
        }
    }

    if (entriesWithoutChildren > 0) {
        qCDebug(ARK) << "Moving" << files << "to" << destination;
    } else {
        qCDebug(ARK) << "Copying " << files << "to" << destination;
    }

    KJob *job;
    if (entriesWithoutChildren != 0) {
        job = m_model->moveFiles(files, destination, CompressionOptions());
    } else {
        job = m_model->copyFiles(files, destination, CompressionOptions());
    }

    if (job) {
        connect(job, &KJob::result,
                this, &Part::slotPasteFilesDone);
        registerJob(job);
        job->start();
    } else {
        delete m_destination;
    }
}
