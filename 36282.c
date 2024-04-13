static inline struct kiocb *aio_get_req(struct kioctx *ctx,
					struct kiocb_batch *batch)
{
	struct kiocb *req;

	if (list_empty(&batch->head))
		if (kiocb_batch_refill(ctx, batch) == 0)
			return NULL;
	req = list_first_entry(&batch->head, struct kiocb, ki_batch);
	list_del(&req->ki_batch);
	return req;
}
