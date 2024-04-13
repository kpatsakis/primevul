void HTMLTextAreaElement::handleBeforeTextInsertedEvent(BeforeTextInsertedEvent* event) const
{
    ASSERT(event);
    ASSERT(renderer());
    int signedMaxLength = maxLength();
    if (signedMaxLength < 0)
        return;
    unsigned unsignedMaxLength = static_cast<unsigned>(signedMaxLength);

    const String& currentValue = innerTextValue();
    unsigned currentLength = computeLengthForSubmission(currentValue);
    if (currentLength + computeLengthForSubmission(event->text()) < unsignedMaxLength)
        return;

    unsigned selectionLength = focused() ? computeLengthForSubmission(plainText(document().frame()->selection().selection().toNormalizedRange().get())) : 0;
    ASSERT(currentLength >= selectionLength);
    unsigned baseLength = currentLength - selectionLength;
    unsigned appendableLength = unsignedMaxLength > baseLength ? unsignedMaxLength - baseLength : 0;
    event->setText(sanitizeUserInputValue(event->text(), appendableLength));
}
