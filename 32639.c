void Part::loadArchive()
{
    const QString fixedMimeType = arguments().metaData()[QStringLiteral("fixedMimeType")];
    auto job = m_model->loadArchive(localFilePath(), fixedMimeType, m_model);

    if (job) {
        registerJob(job);
        job->start();
    } else {
        updateActions();
    }
}
