void HTMLInputElement::updateFocusAppearance(bool restorePreviousSelection)
{
    if (isTextField()) {
        if (!restorePreviousSelection || !hasCachedSelection())
            select();
        else
            restoreCachedSelection();
        if (document().frame())
            document().frame()->selection().revealSelection();
    } else
        HTMLTextFormControlElement::updateFocusAppearance(restorePreviousSelection);
}
