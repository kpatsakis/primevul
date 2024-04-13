Part::~Part()
{
    qDeleteAll(m_tmpExtractDirList);

    if (m_showInfoPanelAction->isChecked()) {
        ArkSettings::setSplitterSizes(m_splitter->sizes());
    }
    ArkSettings::setShowInfoPanel(m_showInfoPanelAction->isChecked());
    ArkSettings::self()->save();

    m_extractArchiveAction->menu()->deleteLater();
    m_extractAction->menu()->deleteLater();
}
