void Part::updateQuickExtractMenu(QAction *extractAction)
{
    if (!extractAction) {
        return;
    }

    QMenu *menu = extractAction->menu();

    if (!menu) {
        menu = new QMenu();
        extractAction->setMenu(menu);
        connect(menu, &QMenu::triggered,
                this, &Part::slotQuickExtractFiles);

        QAction *extractTo = menu->addAction(i18n("Extract To..."));
        extractTo->setIcon(extractAction->icon());
        extractTo->setToolTip(extractAction->toolTip());

        if (extractAction == m_extractArchiveAction) {
            connect(extractTo, &QAction::triggered,
                    this, &Part::slotExtractArchive);
        } else {
            connect(extractTo, &QAction::triggered,
                    this, &Part::slotShowExtractionDialog);
        }

        menu->addSeparator();

        QAction *header = menu->addAction(i18n("Quick Extract To..."));
        header->setEnabled(false);
        header->setIcon(QIcon::fromTheme(QStringLiteral("archive-extract")));
    }

    while (menu->actions().size() > 3) {
        menu->removeAction(menu->actions().last());
    }

    const KConfigGroup conf(KSharedConfig::openConfig(), "ExtractDialog");
    const QStringList dirHistory = conf.readPathEntry("DirHistory", QStringList());

    for (int i = 0; i < qMin(10, dirHistory.size()); ++i) {
        const QString dir = QUrl(dirHistory.value(i)).toString(QUrl::RemoveScheme | QUrl::NormalizePathSegments | QUrl::PreferLocalFile);
        if (QDir(dir).exists()) {
            QAction *newAction = menu->addAction(dir);
            newAction->setData(dir);
        }
    }
}
