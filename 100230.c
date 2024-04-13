void FrameLoader::finishedLoadingDocument(DocumentLoader* loader)
{
#if PLATFORM(WIN) || PLATFORM(CHROMIUM)
    if (m_creatingInitialEmptyDocument)
        return;
#endif
    
    const String& responseMIMEType = loader->responseMIMEType();

#if PLATFORM(MAC)
    m_client->finishedLoading(loader);
    if (!ArchiveFactory::isArchiveMimeType(responseMIMEType))
        return;
#else
    if (!ArchiveFactory::isArchiveMimeType(responseMIMEType)) {
        m_client->finishedLoading(loader);
        return;
    }
#endif
        
    RefPtr<Archive> archive(ArchiveFactory::create(loader->mainResourceData().get(), responseMIMEType));
    if (!archive)
        return;

    loader->addAllArchiveResources(archive.get());
    
    ArchiveResource* mainResource = archive->mainResource();
    loader->setParsedArchiveData(mainResource->data());

    m_responseMIMEType = mainResource->mimeType();

    closeURL();
    didOpenURL(mainResource->url());

    String userChosenEncoding = documentLoader()->overrideEncoding();
    bool encodingIsUserChosen = !userChosenEncoding.isNull();
    setEncoding(encodingIsUserChosen ? userChosenEncoding : mainResource->textEncoding(), encodingIsUserChosen);

    ASSERT(m_frame->document());

    addData(mainResource->data()->data(), mainResource->data()->size());
}
