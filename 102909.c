_XcursorCommentLength (XcursorComment	    *comment)
{
    return XCURSOR_COMMENT_HEADER_LEN + strlen (comment->comment);
}
