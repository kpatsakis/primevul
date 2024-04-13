static inline bool isGoodKey(const typename HashMap::KeyType& key)
{
    return key != HashTraits<typename HashMap::KeyType>::emptyValue() && !HashTraits<typename HashMap::KeyType>::isDeletedValue(key);
}
