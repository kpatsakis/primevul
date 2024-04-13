of2str(int of)
{
    int s, h, m;

    decode_offset(of, s, h, m);
    return rb_enc_sprintf(rb_usascii_encoding(), "%c%02d:%02d", s, h, m);
}