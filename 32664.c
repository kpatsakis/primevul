void Part::slotPasteFiles()
{
    m_destination = (m_view->selectionModel()->selectedRows().count() > 0)
                    ? m_model->entryForIndex(m_view->selectionModel()->currentIndex())
                    : Q_NULLPTR;
    if (m_destination == Q_NULLPTR) {
        m_destination = new Archive::Entry(Q_NULLPTR, QString());
    } else {
        m_destination = new Archive::Entry(Q_NULLPTR, m_destination->fullPath());
    }

    if (m_model->filesToMove.count() > 0) {
        QVector<Archive::Entry*> entriesWithoutChildren = ReadOnlyArchiveInterface::entriesWithoutChildren(QVector<Archive::Entry*>::fromList(m_model->filesToMove.values()));
        if (entriesWithoutChildren.count() == 1) {
            const Archive::Entry *entry = entriesWithoutChildren.first();
            auto entryName = entry->name();
            if (entry->isDir()) {
                entryName += QLatin1Char('/');
            }
            m_destination->setFullPath(m_destination->fullPath() + entryName);
        }

        foreach (const Archive::Entry *entry, entriesWithoutChildren) {
            if (entry->isDir() && m_destination->fullPath().startsWith(entry->fullPath())) {
                KMessageBox::error(widget(),
                                   i18n("Folders can't be moved into themselves."),
                                   i18n("Moving a folder into itself"));
                delete m_destination;
                return;
            }
        }
        auto entryList = QVector<Archive::Entry*>::fromList(m_model->filesToMove.values());
        slotPasteFiles(entryList, m_destination, entriesWithoutChildren.count());
        m_model->filesToMove.clear();
    } else {
        auto entryList = QVector<Archive::Entry*>::fromList(m_model->filesToCopy.values());
        slotPasteFiles(entryList, m_destination, 0);
        m_model->filesToCopy.clear();
    }
    m_cutIndexes.clear();
    updateActions();
}
