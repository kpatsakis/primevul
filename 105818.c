int virtqueue_pop(VirtQueue *vq, VirtQueueElement *elem)
{
    unsigned int i, head, max;
    hwaddr desc_pa = vq->vring.desc;

    if (!virtqueue_num_heads(vq, vq->last_avail_idx))
        return 0;

    /* When we start there are none of either input nor output. */
    elem->out_num = elem->in_num = 0;

    max = vq->vring.num;

    i = head = virtqueue_get_head(vq, vq->last_avail_idx++);
    if (vq->vdev->guest_features & (1 << VIRTIO_RING_F_EVENT_IDX)) {
        vring_avail_event(vq, vring_avail_idx(vq));
    }

    if (vring_desc_flags(desc_pa, i) & VRING_DESC_F_INDIRECT) {
        if (vring_desc_len(desc_pa, i) % sizeof(VRingDesc)) {
            error_report("Invalid size for indirect buffer table");
            exit(1);
        }

        /* loop over the indirect descriptor table */
        max = vring_desc_len(desc_pa, i) / sizeof(VRingDesc);
        desc_pa = vring_desc_addr(desc_pa, i);
        i = 0;
    }

    /* Collect all the descriptors */
    do {
        struct iovec *sg;

        if (vring_desc_flags(desc_pa, i) & VRING_DESC_F_WRITE) {
            if (elem->in_num >= ARRAY_SIZE(elem->in_sg)) {
                error_report("Too many write descriptors in indirect table");
                exit(1);
            }
            elem->in_addr[elem->in_num] = vring_desc_addr(desc_pa, i);
            sg = &elem->in_sg[elem->in_num++];
        } else {
            if (elem->out_num >= ARRAY_SIZE(elem->out_sg)) {
                error_report("Too many read descriptors in indirect table");
                exit(1);
            }
            elem->out_addr[elem->out_num] = vring_desc_addr(desc_pa, i);
            sg = &elem->out_sg[elem->out_num++];
        }

        sg->iov_len = vring_desc_len(desc_pa, i);

        /* If we've got too many, that implies a descriptor loop. */
        if ((elem->in_num + elem->out_num) > max) {
            error_report("Looped descriptor");
            exit(1);
        }
    } while ((i = virtqueue_next_desc(desc_pa, i, max)) != max);

    /* Now map what we have collected */
    virtqueue_map_sg(elem->in_sg, elem->in_addr, elem->in_num, 1);
    virtqueue_map_sg(elem->out_sg, elem->out_addr, elem->out_num, 0);

    elem->index = head;

    vq->inuse++;

    trace_virtqueue_pop(vq, elem, elem->in_num, elem->out_num);
    return elem->in_num + elem->out_num;
}
