void Part::slotSaveAs()
{
    QUrl saveUrl = QFileDialog::getSaveFileUrl(widget(), i18nc("@title:window", "Save Archive As"), url());

    if ((saveUrl.isValid()) && (!saveUrl.isEmpty())) {
        auto statJob = KIO::stat(saveUrl, KIO::StatJob::DestinationSide, 0);
        KJobWidgets::setWindow(statJob, widget());
        if (statJob->exec()) {
            int overwrite = KMessageBox::warningContinueCancel(widget(),
                                                               xi18nc("@info", "An archive named <filename>%1</filename> already exists. Are you sure you want to overwrite it?", saveUrl.fileName()),
                                                               QString(),
                                                               KStandardGuiItem::overwrite());

            if (overwrite != KMessageBox::Continue) {
                return;
            }
        }

        QUrl srcUrl = QUrl::fromLocalFile(localFilePath());

        if (!QFile::exists(localFilePath())) {
            if (url().isLocalFile()) {
                KMessageBox::error(widget(),
                                   xi18nc("@info", "The archive <filename>%1</filename> cannot be copied to the specified location. The archive does not exist anymore.", localFilePath()));

                return;
            } else {
                srcUrl = url();
            }
        }

        KIO::Job *copyJob = KIO::file_copy(srcUrl, saveUrl, -1, KIO::Overwrite);

        KJobWidgets::setWindow(copyJob, widget());
        copyJob->exec();
        if (copyJob->error()) {
            KMessageBox::error(widget(),
                               xi18nc("@info", "The archive could not be saved as <filename>%1</filename>. Try saving it to another location.", saveUrl.path()));
        }
    }
}
