static struct kioctx *ioctx_alloc(unsigned nr_events)
{
	struct mm_struct *mm;
	struct kioctx *ctx;
	int did_sync = 0;

	/* Prevent overflows */
	if ((nr_events > (0x10000000U / sizeof(struct io_event))) ||
	    (nr_events > (0x10000000U / sizeof(struct kiocb)))) {
		pr_debug("ENOMEM: nr_events too high\n");
		return ERR_PTR(-EINVAL);
	}

	if ((unsigned long)nr_events > aio_max_nr)
		return ERR_PTR(-EAGAIN);

	ctx = kmem_cache_zalloc(kioctx_cachep, GFP_KERNEL);
	if (!ctx)
		return ERR_PTR(-ENOMEM);

	ctx->max_reqs = nr_events;
	mm = ctx->mm = current->mm;
	atomic_inc(&mm->mm_count);

	atomic_set(&ctx->users, 1);
	spin_lock_init(&ctx->ctx_lock);
	spin_lock_init(&ctx->ring_info.ring_lock);
	init_waitqueue_head(&ctx->wait);

	INIT_LIST_HEAD(&ctx->active_reqs);
	INIT_LIST_HEAD(&ctx->run_list);
	INIT_DELAYED_WORK(&ctx->wq, aio_kick_handler);

	if (aio_setup_ring(ctx) < 0)
		goto out_freectx;

	/* limit the number of system wide aios */
	do {
		spin_lock_bh(&aio_nr_lock);
		if (aio_nr + nr_events > aio_max_nr ||
		    aio_nr + nr_events < aio_nr)
			ctx->max_reqs = 0;
		else
			aio_nr += ctx->max_reqs;
		spin_unlock_bh(&aio_nr_lock);
		if (ctx->max_reqs || did_sync)
			break;

		/* wait for rcu callbacks to have completed before giving up */
		synchronize_rcu();
		did_sync = 1;
		ctx->max_reqs = nr_events;
	} while (1);

	if (ctx->max_reqs == 0)
		goto out_cleanup;

	/* now link into global list. */
	spin_lock(&mm->ioctx_lock);
	hlist_add_head_rcu(&ctx->list, &mm->ioctx_list);
	spin_unlock(&mm->ioctx_lock);

	dprintk("aio: allocated ioctx %p[%ld]: mm=%p mask=0x%x\n",
		ctx, ctx->user_id, current->mm, ctx->ring_info.nr);
	return ctx;

out_cleanup:
	__put_ioctx(ctx);
	return ERR_PTR(-EAGAIN);

out_freectx:
	mmdrop(mm);
	kmem_cache_free(kioctx_cachep, ctx);
	ctx = ERR_PTR(-ENOMEM);

	dprintk("aio: error allocating ioctx %p\n", ctx);
	return ctx;
}
