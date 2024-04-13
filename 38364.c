static void qeth_clear_output_buffer(struct qeth_qdio_out_q *queue,
		struct qeth_qdio_out_buffer *buf,
		enum qeth_qdio_buffer_states newbufstate)
{
	int i;

	/* is PCI flag set on buffer? */
	if (buf->buffer->element[0].sflags & SBAL_SFLAGS0_PCI_REQ)
		atomic_dec(&queue->set_pci_flags_count);

	if (newbufstate == QETH_QDIO_BUF_EMPTY) {
		qeth_release_skbs(buf);
	}
	for (i = 0; i < QETH_MAX_BUFFER_ELEMENTS(queue->card); ++i) {
		if (buf->buffer->element[i].addr && buf->is_header[i])
			kmem_cache_free(qeth_core_header_cache,
				buf->buffer->element[i].addr);
		buf->is_header[i] = 0;
		buf->buffer->element[i].length = 0;
		buf->buffer->element[i].addr = NULL;
		buf->buffer->element[i].eflags = 0;
		buf->buffer->element[i].sflags = 0;
	}
	buf->buffer->element[15].eflags = 0;
	buf->buffer->element[15].sflags = 0;
	buf->next_element_to_fill = 0;
	atomic_set(&buf->state, newbufstate);
}
