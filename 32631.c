void Part::displayMsgWidget(KMessageWidget::MessageType type, const QString& msg)
{
    m_messageWidget->hide();
    m_messageWidget->setText(msg);
    m_messageWidget->setMessageType(type);
    m_messageWidget->animatedShow();
}
