void Part::slotShowExtractionDialog()
{
    if (!m_model) {
        return;
    }

    QPointer<Kerfuffle::ExtractionDialog> dialog(new Kerfuffle::ExtractionDialog);

    dialog.data()->setModal(true);

    if (m_view->selectionModel()->selectedRows().count() > 0) {
        dialog.data()->setShowSelectedFiles(true);
    }

    dialog.data()->setSingleFolderArchive(isSingleFolderArchive());
    dialog.data()->setSubfolder(detectSubfolder());

    dialog.data()->setCurrentUrl(QUrl::fromLocalFile(QFileInfo(m_model->archive()->fileName()).absolutePath()));

    dialog.data()->show();
    dialog.data()->restoreWindowSize();

    if (dialog.data()->exec()) {

        updateQuickExtractMenu(m_extractArchiveAction);
        updateQuickExtractMenu(m_extractAction);

        QVector<Archive::Entry*> files;

        if (!dialog.data()->extractAllFiles()) {
            files = filesAndRootNodesForIndexes(addChildren(m_view->selectionModel()->selectedRows()));
        }

        qCDebug(ARK) << "Selected " << files;

        Kerfuffle::ExtractionOptions options;
        options.setPreservePaths(dialog->preservePaths());

        const QString destinationDirectory = dialog.data()->destinationDirectory().toLocalFile();
        ExtractJob *job = m_model->extractFiles(files, destinationDirectory, options);
        registerJob(job);

        connect(job, &KJob::result,
                this, &Part::slotExtractionDone);

        job->start();
    }

    delete dialog.data();
}
