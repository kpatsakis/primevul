static void pmcraid_free_sglist(struct pmcraid_sglist *sglist)
{
	int i;

	for (i = 0; i < sglist->num_sg; i++)
		__free_pages(sg_page(&(sglist->scatterlist[i])),
			     sglist->order);

	kfree(sglist);
}
