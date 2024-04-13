void HTMLTextAreaElement::subtreeHasChanged()
{
    setChangedSinceLastFormControlChangeEvent(true);
    setFormControlValueMatchesRenderer(false);
    setNeedsValidityCheck();

    if (!focused())
        return;

    calculateAndAdjustDirectionality();
}
