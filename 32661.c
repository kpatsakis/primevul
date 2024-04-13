void Part::slotLoadingFinished(KJob *job)
{
    if (job->error()) {
        if (arguments().metaData()[QStringLiteral("createNewArchive")] != QLatin1String("true")) {
            if (job->error() != KJob::KilledJobError) {
                displayMsgWidget(KMessageWidget::Error, xi18nc("@info", "Loading the archive <filename>%1</filename> failed with the following error:<nl/><message>%2</message>",
                                                               localFilePath(),
                                                               job->errorString()));
            }

            m_model->reset();
            m_infoPanel->setPrettyFileName(QString());
            m_infoPanel->updateWithDefaults();

            emit setWindowCaption(QString());
        }
    }

    m_view->sortByColumn(0, Qt::AscendingOrder);

    if (m_view->model()->rowCount() == 1) {
        m_view->expandToDepth(0);
    }

    m_view->header()->resizeSections(QHeaderView::ResizeToContents);
    m_view->setDropsEnabled(true);

    updateActions();

    if (!m_model->archive()) {
        return;
    }

    if (!m_model->archive()->comment().isEmpty()) {
        m_commentView->setPlainText(m_model->archive()->comment());
        slotShowComment();
    } else {
        m_commentView->clear();
        m_commentBox->hide();
    }

    if (m_model->rowCount() == 0) {
        qCWarning(ARK) << "No entry listed by the plugin";
        displayMsgWidget(KMessageWidget::Warning, xi18nc("@info", "The archive is empty or Ark could not open its content."));
    } else if (m_model->rowCount() == 1) {
        if (m_model->archive()->mimeType().inherits(QStringLiteral("application/x-cd-image")) &&
            m_model->entryForIndex(m_model->index(0, 0))->fullPath() == QLatin1String("README.TXT")) {
            qCWarning(ARK) << "Detected ISO image with UDF filesystem";
            displayMsgWidget(KMessageWidget::Warning, xi18nc("@info", "Ark does not currently support ISO files with UDF filesystem."));
        }
    }

    if (arguments().metaData()[QStringLiteral("showExtractDialog")] == QLatin1String("true")) {
        QTimer::singleShot(0, this, &Part::slotShowExtractionDialog);
    }
}
