dump_mask_stack(pdf14_mask_t *mask_stack)
{
    pdf14_mask_t *curr_mask = mask_stack;
    int level = 0;

    while (curr_mask != NULL) {
        if_debug1m('v', curr_mask->memory, "[v]mask_level, %d\n", level);
        if_debug1m('v', curr_mask->memory, "[v]mask_buf, %x\n", curr_mask->rc_mask->mask_buf);
        if_debug1m('v', curr_mask->memory, "[v]rc_count, %d\n", curr_mask->rc_mask->rc);
        level++;
        curr_mask = curr_mask->previous;
    }
}
