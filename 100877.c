void HTMLInputElement::endEditing()
{
    ASSERT(document().isActive());
    if (!document().isActive())
        return;

    if (!isTextField())
        return;

    Frame* frame = document().frame();
    frame->spellChecker().didEndEditingOnTextField(this);
    frame->host()->chrome().client().didEndEditingOnTextField(*this);
}
