bool Document::execCommand(const String& commandName, bool, const String& value, ExceptionState& exceptionState)
{
    if (!isHTMLDocument() && !isXHTMLDocument()) {
        exceptionState.throwDOMException(InvalidStateError, "execCommand is only supported on HTML documents.");
        return false;
    }
    if (focusedElement() && isHTMLTextFormControlElement(*focusedElement()))
        UseCounter::count(*this, UseCounter::ExecCommandOnInputOrTextarea);

    if (m_isRunningExecCommand) {
        String message = "We don't execute document.execCommand() this time, because it is called recursively.";
        addConsoleMessage(ConsoleMessage::create(JSMessageSource, WarningMessageLevel, message));
        return false;
    }
    TemporaryChange<bool> executeScope(m_isRunningExecCommand, true);

    EventQueueScope eventQueueScope;
    Editor::tidyUpHTMLStructure(*this);
    Editor::Command editorCommand = command(this, commandName);

    DEFINE_STATIC_LOCAL(SparseHistogram, editorCommandHistogram, ("WebCore.Document.execCommand"));
    editorCommandHistogram.sample(editorCommand.idForHistogram());
    return editorCommand.execute(value);
}
