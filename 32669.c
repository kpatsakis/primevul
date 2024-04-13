void Part::slotRenameFile(const QString &name)
{
    if (name == QLatin1String(".") || name == QLatin1String("..") || name.contains(QLatin1Char('/'))) {
        displayMsgWidget(KMessageWidget::Error, i18n("Filename can't contain slashes and can't be equal to \".\" or \"..\""));
        return;
    }
    const Archive::Entry *entry = m_model->entryForIndex(m_view->selectionModel()->currentIndex());
    QVector<Archive::Entry*> entriesToMove = filesForIndexes(addChildren(m_view->selectionModel()->selectedRows()));

    m_destination = new Archive::Entry();
    const QString &entryPath = entry->fullPath(NoTrailingSlash);
    const QString rootPath = entryPath.left(entryPath.count() - entry->name().count());
    auto path = rootPath + name;
    if (entry->isDir()) {
        path += QLatin1Char('/');
    }
    m_destination->setFullPath(path);

    slotPasteFiles(entriesToMove, m_destination, 1);
}
