void Part::slotPreviewExtractedEntry(KJob *job)
{
    if (!job->error()) {
        PreviewJob *previewJob = qobject_cast<PreviewJob*>(job);
        Q_ASSERT(previewJob);

        m_tmpExtractDirList << previewJob->tempDir();
        ArkViewer::view(previewJob->validatedFilePath());

    } else if (job->error() != KJob::KilledJobError) {
        KMessageBox::error(widget(), job->errorString());
    }
    setReadyGui();
}
