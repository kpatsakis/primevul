void Part::setupActions()
{
    m_signalMapper = new QSignalMapper(this);

    m_showInfoPanelAction = new KToggleAction(i18nc("@action:inmenu", "Show Information Panel"), this);
    actionCollection()->addAction(QStringLiteral( "show-infopanel" ), m_showInfoPanelAction);
    m_showInfoPanelAction->setChecked(ArkSettings::showInfoPanel());
    connect(m_showInfoPanelAction, &QAction::triggered,
            this, &Part::slotToggleInfoPanel);

    m_saveAsAction = actionCollection()->addAction(KStandardAction::SaveAs, QStringLiteral("ark_file_save_as"), this, SLOT(slotSaveAs()));

    m_openFileAction = actionCollection()->addAction(QStringLiteral("openfile"));
    m_openFileAction->setText(i18nc("open a file with external program", "&Open"));
    m_openFileAction->setIcon(QIcon::fromTheme(QStringLiteral("document-open")));
    m_openFileAction->setToolTip(i18nc("@info:tooltip", "Click to open the selected file with the associated application"));
    connect(m_openFileAction, &QAction::triggered, m_signalMapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
    m_signalMapper->setMapping(m_openFileAction, OpenFile);

    m_openFileWithAction = actionCollection()->addAction(QStringLiteral("openfilewith"));
    m_openFileWithAction->setText(i18nc("open a file with external program", "Open &With..."));
    m_openFileWithAction->setIcon(QIcon::fromTheme(QStringLiteral("document-open")));
    m_openFileWithAction->setToolTip(i18nc("@info:tooltip", "Click to open the selected file with an external program"));
    connect(m_openFileWithAction, &QAction::triggered, m_signalMapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
    m_signalMapper->setMapping(m_openFileWithAction, OpenFileWith);

    m_previewAction = actionCollection()->addAction(QStringLiteral("preview"));
    m_previewAction->setText(i18nc("to preview a file inside an archive", "Pre&view"));
    m_previewAction->setIcon(QIcon::fromTheme(QStringLiteral("document-preview-archive")));
    m_previewAction->setToolTip(i18nc("@info:tooltip", "Click to preview the selected file"));
    actionCollection()->setDefaultShortcut(m_previewAction, Qt::CTRL + Qt::Key_P);
    connect(m_previewAction, &QAction::triggered, m_signalMapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
    m_signalMapper->setMapping(m_previewAction, Preview);

    m_extractArchiveAction = actionCollection()->addAction(QStringLiteral("extract_all"));
    m_extractArchiveAction->setText(i18nc("@action:inmenu", "E&xtract All"));
    m_extractArchiveAction->setIcon(QIcon::fromTheme(QStringLiteral("archive-extract")));
    m_extractArchiveAction->setToolTip(i18n("Click to open an extraction dialog, where you can choose how to extract all the files in the archive"));
    actionCollection()->setDefaultShortcut(m_extractArchiveAction, Qt::CTRL + Qt::SHIFT + Qt::Key_E);
    connect(m_extractArchiveAction, &QAction::triggered, this, &Part::slotExtractArchive);

    m_extractAction = actionCollection()->addAction(QStringLiteral("extract"));
    m_extractAction->setText(i18nc("@action:inmenu", "&Extract"));
    m_extractAction->setIcon(QIcon::fromTheme(QStringLiteral("archive-extract")));
    actionCollection()->setDefaultShortcut(m_extractAction, Qt::CTRL + Qt::Key_E);
    m_extractAction->setToolTip(i18n("Click to open an extraction dialog, where you can choose to extract either all files or just the selected ones"));
    connect(m_extractAction, &QAction::triggered, this, &Part::slotShowExtractionDialog);

    m_addFilesAction = actionCollection()->addAction(QStringLiteral("add"));
    m_addFilesAction->setIcon(QIcon::fromTheme(QStringLiteral("archive-insert")));
    m_addFilesAction->setText(i18n("Add &Files..."));
    m_addFilesAction->setToolTip(i18nc("@info:tooltip", "Click to add files to the archive"));
    actionCollection()->setDefaultShortcut(m_addFilesAction, Qt::ALT + Qt::Key_A);
    connect(m_addFilesAction, &QAction::triggered, this, static_cast<void (Part::*)()>(&Part::slotAddFiles));

    m_renameFileAction = actionCollection()->addAction(QStringLiteral("rename"));
    m_renameFileAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename")));
    m_renameFileAction->setText(i18n("&Rename"));
    actionCollection()->setDefaultShortcut(m_renameFileAction, Qt::Key_F2);
    m_renameFileAction->setToolTip(i18nc("@info:tooltip", "Click to rename the selected file"));
    connect(m_renameFileAction, &QAction::triggered, this, &Part::slotEditFileName);

    m_deleteFilesAction = actionCollection()->addAction(QStringLiteral("delete"));
    m_deleteFilesAction->setIcon(QIcon::fromTheme(QStringLiteral("archive-remove")));
    m_deleteFilesAction->setText(i18n("De&lete"));
    actionCollection()->setDefaultShortcut(m_deleteFilesAction, Qt::Key_Delete);
    m_deleteFilesAction->setToolTip(i18nc("@info:tooltip", "Click to delete the selected files"));
    connect(m_deleteFilesAction, &QAction::triggered, this, &Part::slotDeleteFiles);

    m_cutFilesAction = actionCollection()->addAction(QStringLiteral("cut"));
    m_cutFilesAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-cut")));
    m_cutFilesAction->setText(i18nc("@action:inmenu", "C&ut"));
    actionCollection()->setDefaultShortcut(m_cutFilesAction, Qt::CTRL + Qt::Key_X);
    m_cutFilesAction->setToolTip(i18nc("@info:tooltip", "Click to cut the selected files"));
    connect(m_cutFilesAction, &QAction::triggered, this, &Part::slotCutFiles);

    m_copyFilesAction = actionCollection()->addAction(QStringLiteral("copy"));
    m_copyFilesAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));
    m_copyFilesAction->setText(i18nc("@action:inmenu", "C&opy"));
    actionCollection()->setDefaultShortcut(m_copyFilesAction, Qt::CTRL + Qt::Key_C);
    m_copyFilesAction->setToolTip(i18nc("@info:tooltip", "Click to copy the selected files"));
    connect(m_copyFilesAction, &QAction::triggered, this, &Part::slotCopyFiles);

    m_pasteFilesAction = actionCollection()->addAction(QStringLiteral("paste"));
    m_pasteFilesAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-paste")));
    m_pasteFilesAction->setText(i18nc("@action:inmenu", "Pa&ste"));
    actionCollection()->setDefaultShortcut(m_pasteFilesAction, Qt::CTRL + Qt::Key_V);
    m_pasteFilesAction->setToolTip(i18nc("@info:tooltip", "Click to paste the files here"));
    connect(m_pasteFilesAction, &QAction::triggered, this, static_cast<void (Part::*)()>(&Part::slotPasteFiles));

    m_propertiesAction = actionCollection()->addAction(QStringLiteral("properties"));
    m_propertiesAction->setIcon(QIcon::fromTheme(QStringLiteral("document-properties")));
    m_propertiesAction->setText(i18nc("@action:inmenu", "&Properties"));
    actionCollection()->setDefaultShortcut(m_propertiesAction, Qt::ALT + Qt::Key_Return);
    m_propertiesAction->setToolTip(i18nc("@info:tooltip", "Click to see properties for archive"));
    connect(m_propertiesAction, &QAction::triggered, this, &Part::slotShowProperties);

    m_editCommentAction = actionCollection()->addAction(QStringLiteral("edit_comment"));
    m_editCommentAction->setIcon(QIcon::fromTheme(QStringLiteral("document-edit")));
    actionCollection()->setDefaultShortcut(m_editCommentAction, Qt::ALT + Qt::Key_C);
    m_editCommentAction->setToolTip(i18nc("@info:tooltip", "Click to add or edit comment"));
    connect(m_editCommentAction, &QAction::triggered, this, &Part::slotShowComment);

    m_testArchiveAction = actionCollection()->addAction(QStringLiteral("test_archive"));
    m_testArchiveAction->setIcon(QIcon::fromTheme(QStringLiteral("checkmark")));
    m_testArchiveAction->setText(i18nc("@action:inmenu", "&Test Integrity"));
    actionCollection()->setDefaultShortcut(m_testArchiveAction, Qt::ALT + Qt::Key_T);
    m_testArchiveAction->setToolTip(i18nc("@info:tooltip", "Click to test the archive for integrity"));
    connect(m_testArchiveAction, &QAction::triggered, this, &Part::slotTestArchive);

    connect(m_signalMapper, static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped),
            this, &Part::slotOpenEntry);

    updateActions();
    updateQuickExtractMenu(m_extractArchiveAction);
    updateQuickExtractMenu(m_extractAction);
}
