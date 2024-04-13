void Part::slotOpenEntry(int mode)
{
    qCDebug(ARK) << "Opening with mode" << mode;

    QModelIndex index = m_view->selectionModel()->currentIndex();
    Archive::Entry *entry = m_model->entryForIndex(index);

    if (entry->isDir()) {
        return;
    }

    if (!entry->property("link").toString().isEmpty()) {
        displayMsgWidget(KMessageWidget::Information, i18n("Ark cannot open symlinks."));
        return;
    }

    if (!entry->fullPath().isEmpty()) {

        m_openFileMode = static_cast<OpenFileMode>(mode);
        KJob *job = Q_NULLPTR;

        if (m_openFileMode == Preview) {
            job = m_model->preview(entry);
            connect(job, &KJob::result, this, &Part::slotPreviewExtractedEntry);
        } else {
            job = (m_openFileMode == OpenFile) ? m_model->open(entry) : m_model->openWith(entry);
            connect(job, &KJob::result, this, &Part::slotOpenExtractedEntry);
        }

        registerJob(job);
        job->start();
    }
}
