void ConvolverNode::setBuffer(AudioBuffer* buffer)
{
    ASSERT(isMainThread());

    if (!buffer)
        return;

    unsigned numberOfChannels = buffer->numberOfChannels();
    size_t bufferLength = buffer->length();

    bool isBufferGood = numberOfChannels > 0 && numberOfChannels <= 4 && bufferLength;
    ASSERT(isBufferGood);
    if (!isBufferGood)
        return;

    RefPtr<AudioBus> bufferBus = AudioBus::create(numberOfChannels, bufferLength, false);
    for (unsigned i = 0; i < numberOfChannels; ++i)
        bufferBus->setChannelMemory(i, buffer->getChannelData(i)->data(), bufferLength);

    bufferBus->setSampleRate(buffer->sampleRate());

    bool useBackgroundThreads = !context()->isOfflineContext();
    OwnPtr<Reverb> reverb = adoptPtr(new Reverb(bufferBus.get(), AudioNode::ProcessingSizeInFrames, MaxFFTSize, 2, useBackgroundThreads, m_normalize));

    {
        MutexLocker locker(m_processLock);
        m_reverb = reverb.release();
        m_buffer = buffer;
    }
}
