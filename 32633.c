QVector<Kerfuffle::Archive::Entry*> Part::filesAndRootNodesForIndexes(const QModelIndexList& list) const
{
    QVector<Kerfuffle::Archive::Entry*> fileList;
    QStringList fullPathsList;

    foreach (const QModelIndex& index, list) {

        QModelIndex selectionRoot = index.parent();
        while (m_view->selectionModel()->isSelected(selectionRoot) ||
               list.contains(selectionRoot)) {
            selectionRoot = selectionRoot.parent();
        }

        const QString rootFileName = selectionRoot.isValid()
            ? m_model->entryForIndex(selectionRoot)->fullPath()
            : QString();


        QModelIndexList alist = QModelIndexList() << index;
        foreach (Archive::Entry *entry, filesForIndexes(alist)) {
            const QString fullPath = entry->fullPath();
            if (!fullPathsList.contains(fullPath)) {
                entry->rootNode = rootFileName;
                fileList.append(entry);
                fullPathsList.append(fullPath);
            }
        }
    }
    return fileList;
}
