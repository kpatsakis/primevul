static void vnc_led_state_change(VncState *vs)
{
    int ledstate = 0;

    if (!vnc_has_feature(vs, VNC_FEATURE_LED_STATE)) {
        return;
    }

    ledstate = current_led_state(vs);
    vnc_lock_output(vs);
    vnc_write_u8(vs, VNC_MSG_SERVER_FRAMEBUFFER_UPDATE);
    vnc_write_u8(vs, 0);
    vnc_write_u16(vs, 1);
    vnc_framebuffer_update(vs, 0, 0, 1, 1, VNC_ENCODING_LED_STATE);
    vnc_write_u8(vs, ledstate);
    vnc_unlock_output(vs);
    vnc_flush(vs);
}
