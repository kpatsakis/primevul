void Part::extractSelectedFilesTo(const QString& localPath)
{
    if (!m_model) {
        return;
    }

    const QUrl url = QUrl::fromUserInput(localPath, QString());
    KIO::StatJob* statJob = nullptr;

    if (!url.isLocalFile() && !url.scheme().isEmpty()) {
        statJob = KIO::mostLocalUrl(url);

        if (!statJob->exec() || statJob->error() != 0) {
            return;
        }
    }

    const QString destination = statJob ? statJob->statResult().stringValue(KIO::UDSEntry::UDS_LOCAL_PATH) : localPath;
    delete statJob;

    if (!url.isLocalFile() && destination.isEmpty()) {
        qCWarning(ARK) << "Ark cannot extract to non-local destination:" << localPath;
        KMessageBox::sorry(widget(), xi18nc("@info", "Ark can only extract to local destinations."));
        return;
    }

    qCDebug(ARK) << "Extract to" << destination;

    Kerfuffle::ExtractionOptions options;
    options.setDragAndDropEnabled(true);

    ExtractJob *job = m_model->extractFiles(filesAndRootNodesForIndexes(addChildren(m_view->selectionModel()->selectedRows())), destination, options);
    registerJob(job);
    connect(job, &KJob::result,
            this, &Part::slotExtractionDone);
    job->start();
}
