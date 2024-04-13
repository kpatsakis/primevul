void Part::slotToggleInfoPanel(bool visible)
{
    if (visible) {
        m_splitter->setSizes(ArkSettings::splitterSizes());
        m_infoPanel->show();
    } else {
        ArkSettings::setSplitterSizes(m_splitter->sizes());
        m_infoPanel->hide();
    }
}
