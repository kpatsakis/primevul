void Part::slotQuickExtractFiles(QAction *triggeredAction)
{
    if (!triggeredAction->data().isNull()) {
        QString userDestination = triggeredAction->data().toString();
        QString finalDestinationDirectory;
        const QString detectedSubfolder = detectSubfolder();
        qCDebug(ARK) << "Detected subfolder" << detectedSubfolder;

        if (!isSingleFolderArchive()) {
            if (!userDestination.endsWith(QDir::separator())) {
                userDestination.append(QDir::separator());
            }
            finalDestinationDirectory = userDestination + detectedSubfolder;
            QDir(userDestination).mkdir(detectedSubfolder);
        } else {
            finalDestinationDirectory = userDestination;
        }

        qCDebug(ARK) << "Extracting to:" << finalDestinationDirectory;

        ExtractJob *job = m_model->extractFiles(filesAndRootNodesForIndexes(addChildren(m_view->selectionModel()->selectedRows())), finalDestinationDirectory, ExtractionOptions());
        registerJob(job);

        connect(job, &KJob::result,
                this, &Part::slotExtractionDone);

        job->start();
    }
}
