void Part::slotCommentChanged()
{
    if (!m_model->archive()) {
        return;
    }

    if (m_commentMsgWidget->isHidden() && m_commentView->toPlainText() != m_model->archive()->comment()) {
        m_commentMsgWidget->animatedShow();
    } else if (m_commentMsgWidget->isVisible() && m_commentView->toPlainText() == m_model->archive()->comment()) {
        m_commentMsgWidget->hide();
    }
}
