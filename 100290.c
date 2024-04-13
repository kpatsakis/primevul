void InlineFlowBox::flipLinesInBlockDirection(int lineTop, int lineBottom)
{
    setLogicalTop(lineBottom - (logicalTop() - lineTop) - logicalHeight());
    
    for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
        if (curr->renderer()->isPositioned())
            continue; // Positioned placeholders aren't affected here.
        
        if (curr->isInlineFlowBox())
            static_cast<InlineFlowBox*>(curr)->flipLinesInBlockDirection(lineTop, lineBottom);
        else
            curr->setLogicalTop(lineBottom - (curr->logicalTop() - lineTop) - curr->logicalHeight());
    }
}
