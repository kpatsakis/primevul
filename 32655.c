void Part::slotCopyFiles()
{
    m_model->filesToCopy = ArchiveModel::entryMap(filesForIndexes(addChildren(m_view->selectionModel()->selectedRows())));
    qCDebug(ARK) << "Entries marked to copy:" << m_model->filesToCopy.values();
    foreach (const QModelIndex &row, m_cutIndexes) {
        m_view->dataChanged(row, row);
    }
    m_cutIndexes.clear();
    m_model->filesToMove.clear();
    updateActions();
}
