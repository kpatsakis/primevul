void Part::slotDeleteFiles()
{
    const int selectionsCount = m_view->selectionModel()->selectedRows().count();
    const auto reallyDelete =
        KMessageBox::questionYesNo(widget(),
                                   i18ncp("@info",
                                          "Deleting this file is not undoable. Are you sure you want to do this?",
                                          "Deleting these files is not undoable. Are you sure you want to do this?",
                                          selectionsCount),
                                   i18ncp("@title:window", "Delete File", "Delete Files", selectionsCount),
                                   KStandardGuiItem::del(),
                                   KStandardGuiItem::no(),
                                   QString(),
                                   KMessageBox::Dangerous | KMessageBox::Notify);

    if (reallyDelete == KMessageBox::No) {
        return;
    }

    DeleteJob *job = m_model->deleteFiles(filesForIndexes(addChildren(m_view->selectionModel()->selectedRows())));
    connect(job, &KJob::result,
            this, &Part::slotDeleteFilesDone);
    registerJob(job);
    job->start();
}
