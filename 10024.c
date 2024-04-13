skip_vimgrep_pat(char_u *p, char_u **s, int *flags)
{
    return skip_vimgrep_pat_ext(p, s, flags, NULL, NULL);
}