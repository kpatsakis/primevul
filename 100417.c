void JSArray::copyToRegisters(ExecState* exec, Register* buffer, uint32_t maxSize)
{
    ASSERT(m_storage->m_length >= maxSize);
    UNUSED_PARAM(maxSize);
    WriteBarrier<Unknown>* vector = m_storage->m_vector;
    unsigned vectorEnd = min(maxSize, m_vectorLength);
    unsigned i = 0;
    for (; i < vectorEnd; ++i) {
        WriteBarrier<Unknown>& v = vector[i];
        if (!v)
            break;
        buffer[i] = v.get();
    }

    for (; i < maxSize; ++i)
        buffer[i] = get(exec, i);
}
