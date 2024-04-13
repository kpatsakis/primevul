void ConvolverNode::process(size_t framesToProcess)
{
    AudioBus* outputBus = output(0)->bus();
    ASSERT(outputBus);

    MutexTryLocker tryLocker(m_processLock);
    if (tryLocker.locked()) {
        if (!isInitialized() || !m_reverb.get())
            outputBus->zero();
        else {
            m_reverb->process(input(0)->bus(), outputBus, framesToProcess);
        }
    } else {
        outputBus->zero();
    }
}
