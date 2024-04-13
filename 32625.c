Part::Part(QWidget *parentWidget, QObject *parent, const QVariantList& args)
        : KParts::ReadWritePart(parent),
          m_splitter(Q_NULLPTR),
          m_busy(false),
          m_jobTracker(Q_NULLPTR)
{
    Q_UNUSED(args)
    KAboutData aboutData(QStringLiteral("ark"),
                         i18n("ArkPart"),
                         QStringLiteral("3.0"));
    setComponentData(aboutData, false);

    new DndExtractAdaptor(this);

    const QString pathName = QStringLiteral("/DndExtract/%1").arg(s_instanceCounter++);
    if (!QDBusConnection::sessionBus().registerObject(pathName, this)) {
        qCCritical(ARK) << "Could not register a D-Bus object for drag'n'drop";
    }

    QWidget *mainWidget = new QWidget;
    m_vlayout = new QVBoxLayout;
    m_model = new ArchiveModel(pathName, this);
    m_splitter = new QSplitter(Qt::Horizontal, parentWidget);
    m_view = new ArchiveView;
    m_infoPanel = new InfoPanel(m_model);

    m_commentView = new QPlainTextEdit();
    m_commentView->setReadOnly(true);
    m_commentView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    m_commentBox = new QGroupBox(i18n("Comment"));
    m_commentBox->hide();
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(m_commentView);
    m_commentBox->setLayout(vbox);

    m_messageWidget = new KMessageWidget(parentWidget);
    m_messageWidget->setWordWrap(true);
    m_messageWidget->hide();

    m_commentMsgWidget = new KMessageWidget();
    m_commentMsgWidget->setText(i18n("Comment has been modified."));
    m_commentMsgWidget->setMessageType(KMessageWidget::Information);
    m_commentMsgWidget->setCloseButtonVisible(false);
    m_commentMsgWidget->hide();

    QAction *saveAction = new QAction(i18n("Save"), m_commentMsgWidget);
    m_commentMsgWidget->addAction(saveAction);
    connect(saveAction, &QAction::triggered, this, &Part::slotAddComment);

    m_commentBox->layout()->addWidget(m_commentMsgWidget);

    connect(m_commentView, &QPlainTextEdit::textChanged, this, &Part::slotCommentChanged);

    setWidget(mainWidget);
    mainWidget->setLayout(m_vlayout);

    m_vlayout->setContentsMargins(0,0,0,0);
    m_vlayout->addWidget(m_messageWidget);
    m_vlayout->addWidget(m_splitter);

    m_commentSplitter = new QSplitter(Qt::Vertical, parentWidget);
    m_commentSplitter->setOpaqueResize(false);
    m_commentSplitter->addWidget(m_view);
    m_commentSplitter->addWidget(m_commentBox);
    m_commentSplitter->setCollapsible(0, false);

    m_splitter->addWidget(m_commentSplitter);
    m_splitter->addWidget(m_infoPanel);

    if (!ArkSettings::showInfoPanel()) {
        m_infoPanel->hide();
    } else {
        m_splitter->setSizes(ArkSettings::splitterSizes());
    }

    setupView();
    setupActions();

    connect(m_view, &ArchiveView::entryChanged,
            this, &Part::slotRenameFile);

    connect(m_model, &ArchiveModel::loadingStarted,
            this, &Part::slotLoadingStarted);
    connect(m_model, &ArchiveModel::loadingFinished,
            this, &Part::slotLoadingFinished);
    connect(m_model, &ArchiveModel::droppedFiles,
            this, static_cast<void (Part::*)(const QStringList&, const Archive::Entry*, const QString&)>(&Part::slotAddFiles));
    connect(m_model, &ArchiveModel::error,
            this, &Part::slotError);
    connect(m_model, &ArchiveModel::messageWidget,
            this, &Part::displayMsgWidget);

    connect(this, &Part::busy,
            this, &Part::setBusyGui);
    connect(this, &Part::ready,
            this, &Part::setReadyGui);
    connect(this, static_cast<void (KParts::ReadOnlyPart::*)()>(&KParts::ReadOnlyPart::completed),
            this, &Part::setFileNameFromArchive);

    m_statusBarExtension = new KParts::StatusBarExtension(this);

    setXMLFile(QStringLiteral("ark_part.rc"));
}
