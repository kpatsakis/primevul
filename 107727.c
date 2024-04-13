static void umap_remove_region(struct vm_info *vmi, struct vm_region *reg)
{
	TAILQ_REMOVE(&vmi->regions, reg, link);
	free(reg);
}
