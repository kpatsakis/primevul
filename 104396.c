static const char *code2name(int keycode)
{
    return QKeyCode_lookup[qemu_input_key_number_to_qcode(keycode)];
}
