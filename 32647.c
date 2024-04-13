QList<Kerfuffle::SettingsPage*> Part::settingsPages(QWidget *parent) const
{
    QList<SettingsPage*> pages;
    pages.append(new GeneralSettingsPage(parent, i18nc("@title:tab", "General Settings"), QStringLiteral("go-home")));
    pages.append(new ExtractionSettingsPage(parent, i18nc("@title:tab", "Extraction Settings"), QStringLiteral("archive-extract")));
    pages.append(new PreviewSettingsPage(parent, i18nc("@title:tab", "Preview Settings"), QStringLiteral("document-preview-archive")));

    return pages;
}
