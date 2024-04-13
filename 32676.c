void Part::updateActions()
{
    bool isWritable = m_model->archive() && !m_model->archive()->isReadOnly();
    const Archive::Entry *entry = m_model->entryForIndex(m_view->selectionModel()->currentIndex());
    int selectedEntriesCount = m_view->selectionModel()->selectedRows().count();

    const bool isEncryptedButUnknownPassword = m_model->archive() &&
                                               m_model->archive()->encryptionType() != Archive::Unencrypted &&
                                               m_model->archive()->password().isEmpty();

    if (isEncryptedButUnknownPassword) {
        m_addFilesAction->setToolTip(xi18nc("@info:tooltip",
                                            "Adding files to existing password-protected archives with no header-encryption is currently not supported."
                                            "<nl/><nl/>Extract the files and create a new archive if you want to add files."));
        m_testArchiveAction->setToolTip(xi18nc("@info:tooltip",
                                               "Testing password-protected archives with no header-encryption is currently not supported."));
    } else {
        m_addFilesAction->setToolTip(i18nc("@info:tooltip", "Click to add files to the archive"));
        m_testArchiveAction->setToolTip(i18nc("@info:tooltip", "Click to test the archive for integrity"));
    }

    const int maxPreviewSize = ArkSettings::previewFileSizeLimit() * 1024 * 1024;
    const bool limit = ArkSettings::limitPreviewFileSize();
    bool isPreviewable = (!limit || (limit && entry != Q_NULLPTR && entry->property("size").toLongLong() < maxPreviewSize));

    const bool isDir = (entry == Q_NULLPTR) ? false : entry->isDir();
    m_previewAction->setEnabled(!isBusy() &&
                                isPreviewable &&
                                !isDir &&
                                (selectedEntriesCount == 1));
    m_extractArchiveAction->setEnabled(!isBusy() &&
                                       (m_model->rowCount() > 0));
    m_extractAction->setEnabled(!isBusy() &&
                                (m_model->rowCount() > 0));
    m_saveAsAction->setEnabled(!isBusy() &&
                               m_model->rowCount() > 0);
    m_addFilesAction->setEnabled(!isBusy() &&
                                 isWritable &&
                                 !isEncryptedButUnknownPassword);
    m_deleteFilesAction->setEnabled(!isBusy() &&
                                    isWritable &&
                                    (selectedEntriesCount > 0));
    m_openFileAction->setEnabled(!isBusy() &&
                                 isPreviewable &&
                                 !isDir &&
                                 (selectedEntriesCount == 1));
    m_openFileWithAction->setEnabled(!isBusy() &&
                                     isPreviewable &&
                                     !isDir &&
                                     (selectedEntriesCount == 1));
    m_propertiesAction->setEnabled(!isBusy() &&
                                   m_model->archive());

    m_renameFileAction->setEnabled(!isBusy() &&
                                   isWritable &&
                                   (selectedEntriesCount == 1));
    m_cutFilesAction->setEnabled(!isBusy() &&
                                 isWritable &&
                                 (selectedEntriesCount > 0));
    m_copyFilesAction->setEnabled(!isBusy() &&
                                  isWritable &&
                                  (selectedEntriesCount > 0));
    m_pasteFilesAction->setEnabled(!isBusy() &&
                                   isWritable &&
                                   (selectedEntriesCount == 0 || (selectedEntriesCount == 1 && isDir)) &&
                                   (m_model->filesToMove.count() > 0 || m_model->filesToCopy.count() > 0));

    m_commentView->setEnabled(!isBusy());
    m_commentMsgWidget->setEnabled(!isBusy());

    m_editCommentAction->setEnabled(false);
    m_testArchiveAction->setEnabled(false);

    if (m_model->archive()) {
        const KPluginMetaData metadata = PluginManager().preferredPluginFor(m_model->archive()->mimeType())->metaData();
        bool supportsWriteComment = ArchiveFormat::fromMetadata(m_model->archive()->mimeType(), metadata).supportsWriteComment();
        m_editCommentAction->setEnabled(!isBusy() &&
                                        supportsWriteComment);
        m_commentView->setReadOnly(!supportsWriteComment);
        m_editCommentAction->setText(m_model->archive()->hasComment() ? i18nc("@action:inmenu mutually exclusive with Add &Comment", "Edit &Comment") :
                                                                        i18nc("@action:inmenu mutually exclusive with Edit &Comment", "Add &Comment"));

        bool supportsTesting = ArchiveFormat::fromMetadata(m_model->archive()->mimeType(), metadata).supportsTesting();
        m_testArchiveAction->setEnabled(!isBusy() &&
                                        supportsTesting &&
                                        !isEncryptedButUnknownPassword);
    } else {
        m_commentView->setReadOnly(true);
        m_editCommentAction->setText(i18nc("@action:inmenu mutually exclusive with Edit &Comment", "Add &Comment"));
    }
}
