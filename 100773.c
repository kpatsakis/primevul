void Element::focus(bool restorePreviousSelection, FocusDirection direction)
{
    if (!inDocument())
        return;

    Document* doc = document();
    if (doc->focusedNode() == this)
        return;

    if (doc->haveStylesheetsLoaded()) {
        doc->updateLayoutIgnorePendingStylesheets();
        if (!isFocusable())
            return;
    }

    if (!supportsFocus())
        return;

    RefPtr<Node> protect;
    if (Page* page = doc->page()) {
        protect = this;
        if (!page->focusController()->setFocusedNode(this, doc->frame(), direction))
            return;
    }

    doc->updateLayoutIgnorePendingStylesheets();

    if (!isFocusable()) {
        ensureElementRareData()->setNeedsFocusAppearanceUpdateSoonAfterAttach(true);
        return;
    }
        
    cancelFocusAppearanceUpdate();
    updateFocusAppearance(restorePreviousSelection);
}
