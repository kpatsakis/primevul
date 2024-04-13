void Part::slotAddFilesDone(KJob* job)
{
    qDeleteAll(m_jobTempEntries);
    m_jobTempEntries.clear();
    if (job->error() && job->error() != KJob::KilledJobError) {
        KMessageBox::error(widget(), job->errorString());
    } else {
        m_messageWidget->hide();

        if (m_model->archive()->isMultiVolume()) {
            qCDebug(ARK) << "Multi-volume archive detected, re-opening...";
            KParts::OpenUrlArguments args = arguments();
            args.metaData()[QStringLiteral("createNewArchive")] = QStringLiteral("false");
            setArguments(args);

            openUrl(QUrl::fromLocalFile(m_model->archive()->multiVolumeName()));
        }
    }
    m_cutIndexes.clear();
    m_model->filesToMove.clear();
    m_model->filesToCopy.clear();
}
