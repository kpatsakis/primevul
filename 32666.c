void Part::slotPasteFilesDone(KJob *job)
{
    if (job->error() && job->error() != KJob::KilledJobError) {
        KMessageBox::error(widget(), job->errorString());
    }
    m_cutIndexes.clear();
    m_model->filesToMove.clear();
    m_model->filesToCopy.clear();
}
