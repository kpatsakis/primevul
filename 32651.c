void Part::slotAddComment()
{
    CommentJob *job = m_model->archive()->addComment(m_commentView->toPlainText());
    if (!job) {
        return;
    }
    registerJob(job);
    job->start();
    m_commentMsgWidget->hide();
    if (m_commentView->toPlainText().isEmpty()) {
        m_commentBox->hide();
    }
}
