inline bool SearchBuffer::isBadMatch(const UChar* match, size_t matchLength) const
{
    if (!m_targetRequiresKanaWorkaround)
        return false;

    normalizeCharacters(match, matchLength, m_normalizedMatch);

    const UChar* a = m_normalizedTarget.begin();
    const UChar* aEnd = m_normalizedTarget.end();

    const UChar* b = m_normalizedMatch.begin();
    const UChar* bEnd = m_normalizedMatch.end();

    while (true) {
        while (a != aEnd && !isKanaLetter(*a))
            ++a;
        while (b != bEnd && !isKanaLetter(*b))
            ++b;

        if (a == aEnd || b == bEnd) {
            ASSERT(a == aEnd);
            ASSERT(b == bEnd);
            return false;
        }

        if (isSmallKanaLetter(*a) != isSmallKanaLetter(*b))
            return true;
        if (composedVoicedSoundMark(*a) != composedVoicedSoundMark(*b))
            return true;
        ++a;
        ++b;

        while (1) {
            if (!(a != aEnd && isCombiningVoicedSoundMark(*a))) {
                if (b != bEnd && isCombiningVoicedSoundMark(*b))
                    return true;
                break;
            }
            if (!(b != bEnd && isCombiningVoicedSoundMark(*b)))
                return true;
            if (*a != *b)
                return true;
            ++a;
            ++b;
        }
    }
}
