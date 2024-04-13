static RList *entries(RBinFile *bf) {
	RList *ret;
	RBinObject *obj = bf ? bf->o : NULL;

	if (!obj || !obj->bin_obj || !(ret = r_list_newf (free))) {
		return NULL;
	}

	RKernelCacheObj *kobj = (RKernelCacheObj*) obj->bin_obj;
	ut64 entry_vaddr = kobj->mach0->entry;
	if (kobj->pa2va_exec <= entry_vaddr) {
		ut64 entry_paddr = entry_vaddr - kobj->pa2va_exec;
		RBinAddr *ba = newEntry (entry_paddr, entry_vaddr, 0);
		if (ba) {
			r_list_append (ret, ba);
		}
	}

	process_constructors (kobj, kobj->mach0, ret, 0, true, R_K_CONSTRUCTOR_TO_ENTRY, NULL);

	return ret;
}