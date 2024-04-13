void JSArray::put(ExecState* exec, unsigned i, JSValue value)
{
    checkConsistency();

    ArrayStorage* storage = m_storage;

    unsigned length = storage->m_length;
    if (i >= length && i <= MAX_ARRAY_INDEX) {
        length = i + 1;
        storage->m_length = length;
    }

    if (i < m_vectorLength) {
        WriteBarrier<Unknown>& valueSlot = storage->m_vector[i];
        if (valueSlot) {
            valueSlot.set(exec->globalData(), this, value);
            checkConsistency();
            return;
        }
        valueSlot.set(exec->globalData(), this, value);
        ++storage->m_numValuesInVector;
        checkConsistency();
        return;
    }

    putSlowCase(exec, i, value);
}
