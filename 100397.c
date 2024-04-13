static void computeInlineStaticDistance(Length& logicalLeft, Length& logicalRight, const RenderBox* child, const RenderBoxModelObject* containerBlock, int containerLogicalWidth,
                                        TextDirection containerDirection)
{
    if (!logicalLeft.isAuto() || !logicalRight.isAuto())
        return;

    if (containerDirection == LTR) {
        int staticPosition = child->layer()->staticInlinePosition() - containerBlock->borderLogicalLeft();
        for (RenderObject* curr = child->parent(); curr && curr != containerBlock; curr = curr->container()) {
            if (curr->isBox())
                staticPosition += toRenderBox(curr)->logicalLeft();
        }
        logicalLeft.setValue(Fixed, staticPosition);
    } else {
        RenderBox* enclosingBox = child->parent()->enclosingBox();
        int staticPosition = child->layer()->staticInlinePosition() + containerLogicalWidth + containerBlock->borderLogicalRight();
        staticPosition -= enclosingBox->logicalWidth();
        for (RenderObject* curr = enclosingBox; curr && curr != containerBlock; curr = curr->container()) {
            if (curr->isBox())
                staticPosition -= toRenderBox(curr)->logicalLeft();
        }
        logicalRight.setValue(Fixed, staticPosition);
    }
}
