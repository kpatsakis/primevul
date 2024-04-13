static void update_cursor(VirtIOGPU *g, struct virtio_gpu_update_cursor *cursor)
{
    struct virtio_gpu_scanout *s;
    bool move = cursor->hdr.type == VIRTIO_GPU_CMD_MOVE_CURSOR;

    if (cursor->pos.scanout_id >= g->conf.max_outputs) {
        return;
    }
    s = &g->scanout[cursor->pos.scanout_id];

    trace_virtio_gpu_update_cursor(cursor->pos.scanout_id,
                                   cursor->pos.x,
                                   cursor->pos.y,
                                   move ? "move" : "update",
                                   cursor->resource_id);

    if (!move) {
        if (!s->current_cursor) {
            s->current_cursor = cursor_alloc(64, 64);
        }

        s->current_cursor->hot_x = cursor->hot_x;
        s->current_cursor->hot_y = cursor->hot_y;

        if (cursor->resource_id > 0) {
            VIRGL(g, update_cursor_data_virgl, update_cursor_data_simple,
                  g, s, cursor->resource_id);
        }
        dpy_cursor_define(s->con, s->current_cursor);

        s->cursor = *cursor;
    } else {
        s->cursor.pos.x = cursor->pos.x;
        s->cursor.pos.y = cursor->pos.y;
    }
    dpy_mouse_set(s->con, cursor->pos.x, cursor->pos.y,
                  cursor->resource_id ? 1 : 0);
}
