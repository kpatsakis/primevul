void ConvolverNode::uninitialize()
{
    if (!isInitialized())
        return;

    m_reverb.clear();
    AudioNode::uninitialize();
}
