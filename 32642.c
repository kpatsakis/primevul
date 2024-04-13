void Part::resetGui()
{
    m_messageWidget->hide();
    m_commentView->clear();
    m_commentBox->hide();
    m_infoPanel->setIndex(QModelIndex());
    m_compressionOptions = CompressionOptions();
}
