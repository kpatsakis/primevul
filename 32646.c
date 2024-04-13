void Part::setReadyGui()
{
    QApplication::restoreOverrideCursor();
    m_busy = false;

    if (m_statusBarExtension->statusBar()) {
        m_statusBarExtension->statusBar()->hide();
    }

    m_view->setEnabled(true);
    updateActions();
}
