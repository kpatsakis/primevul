void virtio_gpu_process_cmdq(VirtIOGPU *g)
{
    struct virtio_gpu_ctrl_command *cmd;

    while (!QTAILQ_EMPTY(&g->cmdq)) {
        cmd = QTAILQ_FIRST(&g->cmdq);

        /* process command */
        VIRGL(g, virtio_gpu_virgl_process_cmd, virtio_gpu_simple_process_cmd,
              g, cmd);
        if (cmd->waiting) {
            break;
        }
        QTAILQ_REMOVE(&g->cmdq, cmd, next);
        if (virtio_gpu_stats_enabled(g->conf)) {
            g->stats.requests++;
        }

        if (!cmd->finished) {
            QTAILQ_INSERT_TAIL(&g->fenceq, cmd, next);
            g->inflight++;
            if (virtio_gpu_stats_enabled(g->conf)) {
                if (g->stats.max_inflight < g->inflight) {
                    g->stats.max_inflight = g->inflight;
                }
                fprintf(stderr, "inflight: %3d (+)\r", g->inflight);
            }
        } else {
            g_free(cmd);
        }
    }
}
