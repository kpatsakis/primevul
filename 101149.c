static inline String canonicalizedTitle(Document* document, const String& title)
{
    unsigned length = title.length();
    unsigned builderIndex = 0;
    const CharacterType* characters = title.getCharacters<CharacterType>();

    StringBuffer<CharacterType> buffer(length);

    bool pendingWhitespace = false;
    for (unsigned i = 0; i < length; ++i) {
        UChar32 c = characters[i];
        if (c <= 0x20 || c == 0x7F || (WTF::Unicode::category(c) & (WTF::Unicode::Separator_Line | WTF::Unicode::Separator_Paragraph))) {
            if (builderIndex != 0)
                pendingWhitespace = true;
        } else {
            if (pendingWhitespace) {
                buffer[builderIndex++] = ' ';
                pendingWhitespace = false;
            }
            buffer[builderIndex++] = c;
        }
    }
    buffer.shrink(builderIndex);

    return String::adopt(buffer);
}
