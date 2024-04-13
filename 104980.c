void virtio_queue_aio_set_host_notifier_handler(VirtQueue *vq, AioContext *ctx,
                                                VirtIOHandleOutput handle_output)
{
    if (handle_output) {
        vq->handle_aio_output = handle_output;
        aio_set_event_notifier(ctx, &vq->host_notifier, true,
                               virtio_queue_host_notifier_aio_read);
    } else {
        aio_set_event_notifier(ctx, &vq->host_notifier, true, NULL);
        /* Test and clear notifier before after disabling event,
         * in case poll callback didn't have time to run. */
        virtio_queue_host_notifier_aio_read(&vq->host_notifier);
        vq->handle_aio_output = NULL;
    }
}
