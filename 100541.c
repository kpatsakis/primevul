void EditorClientBlackBerry::undo()
{
    if (canUndo()) {
        EditCommandStack::iterator back = --m_undoStack.end();
        RefPtr<UndoStep> command(*back);
        m_undoStack.remove(back);

        command->unapply();
    }
}
