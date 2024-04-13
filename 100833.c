TextIterator::TextIterator(const Range* r, TextIteratorBehavior behavior)
    : m_startContainer(0)
    , m_startOffset(0)
    , m_endContainer(0)
    , m_endOffset(0)
    , m_positionNode(0)
    , m_textCharacters(0)
    , m_textLength(0)
    , m_remainingTextBox(0)
    , m_firstLetterText(0)
    , m_sortedTextBoxesPosition(0)
    , m_emitsCharactersBetweenAllVisiblePositions(behavior & TextIteratorEmitsCharactersBetweenAllVisiblePositions)
    , m_entersTextControls(behavior & TextIteratorEntersTextControls)
    , m_emitsTextWithoutTranscoding(behavior & TextIteratorEmitsTextsWithoutTranscoding)
    , m_emitsOriginalText(behavior & TextIteratorEmitsOriginalText)
    , m_handledFirstLetter(false)
    , m_ignoresStyleVisibility(behavior & TextIteratorIgnoresStyleVisibility)
    , m_emitsObjectReplacementCharacters(behavior & TextIteratorEmitsObjectReplacementCharacters)
    , m_stopsOnFormControls(behavior & TextIteratorStopsOnFormControls)
    , m_shouldStop(false)
    , m_emitsImageAltText(behavior & TextIteratorEmitsImageAltText)
{
    if (!r)
        return;

    Node* startContainer = r->startContainer();
    if (!startContainer)
        return;
    int startOffset = r->startOffset();
    Node* endContainer = r->endContainer();
    int endOffset = r->endOffset();

    ASSERT(r->boundaryPointsValid());

    m_startContainer = startContainer;
    m_startOffset = startOffset;
    m_endContainer = endContainer;
    m_endOffset = endOffset;

    m_node = r->firstNode();
    if (!m_node)
        return;
    setUpFullyClippedStack(m_fullyClippedStack, m_node);
    m_offset = m_node == m_startContainer ? m_startOffset : 0;
    m_handledNode = false;
    m_handledChildren = false;

    m_pastEndNode = nextInPreOrderCrossingShadowBoundaries(endContainer, endOffset);

    m_needsAnotherNewline = false;
    m_textBox = 0;

    m_hasEmitted = false;
    m_lastTextNode = 0;
    m_lastTextNodeEndedWithCollapsedSpace = false;
    m_lastCharacter = 0;

#ifndef NDEBUG
    m_positionNode = m_node;
#endif

    advance();
}
