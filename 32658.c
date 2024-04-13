void Part::slotEditFileName()
{
    QModelIndex currentIndex = m_view->selectionModel()->currentIndex();
    currentIndex = (currentIndex.parent().isValid())
                   ? currentIndex.parent().child(currentIndex.row(), 0)
                   : m_model->index(currentIndex.row(), 0);
    m_view->openEntryEditor(currentIndex);
}
