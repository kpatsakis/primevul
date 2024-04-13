void RenderBox::computePositionedLogicalHeightUsing(Length logicalHeightLength, const RenderBoxModelObject* containerBlock,
                                                    int containerLogicalHeight, int bordersPlusPadding,
                                                    Length logicalTop, Length logicalBottom, Length marginBefore, Length marginAfter,
                                                    int& logicalHeightValue, int& marginBeforeValue, int& marginAfterValue, int& logicalTopPos)
{
    ASSERT(!(logicalTop.isAuto() && logicalBottom.isAuto()));

    int contentLogicalHeight = logicalHeight() - bordersPlusPadding;

    int logicalTopValue = 0;

    bool logicalHeightIsAuto = logicalHeightLength.isAuto();
    bool logicalTopIsAuto = logicalTop.isAuto();
    bool logicalBottomIsAuto = logicalBottom.isAuto();

    if (isTable()) {
        logicalHeightLength.setValue(Fixed, contentLogicalHeight);
        logicalHeightIsAuto = false;
    }

    if (!logicalTopIsAuto && !logicalHeightIsAuto && !logicalBottomIsAuto) {
        /*-----------------------------------------------------------------------*\
         * If none of the three are 'auto': If both 'margin-top' and 'margin-
         * bottom' are 'auto', solve the equation under the extra constraint that
         * the two margins get equal values. If one of 'margin-top' or 'margin-
         * bottom' is 'auto', solve the equation for that value. If the values
         * are over-constrained, ignore the value for 'bottom' and solve for that
         * value.
        \*-----------------------------------------------------------------------*/

        logicalHeightValue = computeContentBoxLogicalHeight(logicalHeightLength.calcValue(containerLogicalHeight));
        logicalTopValue = logicalTop.calcValue(containerLogicalHeight);

        const int availableSpace = containerLogicalHeight - (logicalTopValue + logicalHeightValue + logicalBottom.calcValue(containerLogicalHeight) + bordersPlusPadding);

        if (marginBefore.isAuto() && marginAfter.isAuto()) {
            marginBeforeValue = availableSpace / 2; // split the difference
            marginAfterValue = availableSpace - marginBeforeValue; // account for odd valued differences
        } else if (marginBefore.isAuto()) {
            marginAfterValue = marginAfter.calcValue(containerLogicalHeight);
            marginBeforeValue = availableSpace - marginAfterValue;
        } else if (marginAfter.isAuto()) {
            marginBeforeValue = marginBefore.calcValue(containerLogicalHeight);
            marginAfterValue = availableSpace - marginBeforeValue;
        } else {
            marginBeforeValue = marginBefore.calcValue(containerLogicalHeight);
            marginAfterValue = marginAfter.calcValue(containerLogicalHeight);
        }
    } else {
        /*--------------------------------------------------------------------*\
         * Otherwise, set 'auto' values for 'margin-top' and 'margin-bottom'
         * to 0, and pick the one of the following six rules that applies.
         *
         * 1. 'top' and 'height' are 'auto' and 'bottom' is not 'auto', then
         *    the height is based on the content, and solve for 'top'.
         *
         *              OMIT RULE 2 AS IT SHOULD NEVER BE HIT
         * ------------------------------------------------------------------
         * 2. 'top' and 'bottom' are 'auto' and 'height' is not 'auto', then
         *    set 'top' to the static position, and solve for 'bottom'.
         * ------------------------------------------------------------------
         *
         * 3. 'height' and 'bottom' are 'auto' and 'top' is not 'auto', then
         *    the height is based on the content, and solve for 'bottom'.
         * 4. 'top' is 'auto', 'height' and 'bottom' are not 'auto', and
         *    solve for 'top'.
         * 5. 'height' is 'auto', 'top' and 'bottom' are not 'auto', and
         *    solve for 'height'.
         * 6. 'bottom' is 'auto', 'top' and 'height' are not 'auto', and
         *    solve for 'bottom'.
        \*--------------------------------------------------------------------*/

        marginBeforeValue = marginBefore.calcMinValue(containerLogicalHeight);
        marginAfterValue = marginAfter.calcMinValue(containerLogicalHeight);

        const int availableSpace = containerLogicalHeight - (marginBeforeValue + marginAfterValue + bordersPlusPadding);

        if (logicalTopIsAuto && logicalHeightIsAuto && !logicalBottomIsAuto) {
            logicalHeightValue = contentLogicalHeight;
            logicalTopValue = availableSpace - (logicalHeightValue + logicalBottom.calcValue(containerLogicalHeight));
        } else if (!logicalTopIsAuto && logicalHeightIsAuto && logicalBottomIsAuto) {
            logicalTopValue = logicalTop.calcValue(containerLogicalHeight);
            logicalHeightValue = contentLogicalHeight;
        } else if (logicalTopIsAuto && !logicalHeightIsAuto && !logicalBottomIsAuto) {
            logicalHeightValue = computeContentBoxLogicalHeight(logicalHeightLength.calcValue(containerLogicalHeight));
            logicalTopValue = availableSpace - (logicalHeightValue + logicalBottom.calcValue(containerLogicalHeight));
        } else if (!logicalTopIsAuto && logicalHeightIsAuto && !logicalBottomIsAuto) {
            logicalTopValue = logicalTop.calcValue(containerLogicalHeight);
            logicalHeightValue = max(0, availableSpace - (logicalTopValue + logicalBottom.calcValue(containerLogicalHeight)));
        } else if (!logicalTopIsAuto && !logicalHeightIsAuto && logicalBottomIsAuto) {
            logicalHeightValue = computeContentBoxLogicalHeight(logicalHeightLength.calcValue(containerLogicalHeight));
            logicalTopValue = logicalTop.calcValue(containerLogicalHeight);
        }
    }

    logicalTopPos = logicalTopValue + marginBeforeValue;
    computeLogicalTopPositionedOffset(logicalTopPos, this, logicalHeightValue, containerBlock, containerLogicalHeight);
}
