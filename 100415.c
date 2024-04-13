void JSArray::checkConsistency(ConsistencyCheckType type)
{
    ArrayStorage* storage = m_storage;

    ASSERT(storage);
    if (type == SortConsistencyCheck)
        ASSERT(!storage->m_sparseValueMap);

    unsigned numValuesInVector = 0;
    for (unsigned i = 0; i < m_vectorLength; ++i) {
        if (JSValue value = storage->m_vector[i]) {
            ASSERT(i < storage->m_length);
            if (type != DestructorConsistencyCheck)
                value.isUndefined(); // Likely to crash if the object was deallocated.
            ++numValuesInVector;
        } else {
            if (type == SortConsistencyCheck)
                ASSERT(i >= storage->m_numValuesInVector);
        }
    }
    ASSERT(numValuesInVector == storage->m_numValuesInVector);
    ASSERT(numValuesInVector <= storage->m_length);

    if (storage->m_sparseValueMap) {
        SparseArrayValueMap::iterator end = storage->m_sparseValueMap->end();
        for (SparseArrayValueMap::iterator it = storage->m_sparseValueMap->begin(); it != end; ++it) {
            unsigned index = it->first;
            ASSERT(index < storage->m_length);
            ASSERT(index >= storage->m_vectorLength);
            ASSERT(index <= MAX_ARRAY_INDEX);
            ASSERT(it->second);
            if (type != DestructorConsistencyCheck)
                it->second.isUndefined(); // Likely to crash if the object was deallocated.
        }
    }
}
