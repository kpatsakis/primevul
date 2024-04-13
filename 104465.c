static void vnc_jobs_bh(void *opaque)
{
    VncState *vs = opaque;

    vnc_jobs_consume_buffer(vs);
}
