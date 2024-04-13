getMyanmarCharClass (HB_UChar16 ch)
{
    if (ch == Mymr_C_SIGN_ZWJ)
        return Mymr_CC_ZERO_WIDTH_J_MARK;

    if (ch == Mymr_C_SIGN_ZWNJ)
        return Mymr_CC_ZERO_WIDTH_NJ_MARK;

    if (ch < 0x1000 || ch > 0x105f)
        return Mymr_CC_RESERVED;

    return mymrCharClasses[ch - 0x1000];
}
