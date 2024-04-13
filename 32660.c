void Part::slotExtractArchive()
{
    if (m_view->selectionModel()->selectedRows().count() > 0) {
        m_view->selectionModel()->clear();
    }

    slotShowExtractionDialog();
}
