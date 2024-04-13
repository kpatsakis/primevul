void update_memslots(struct kvm_memslots *slots, struct kvm_memory_slot *new,
		     u64 last_generation)
{
	if (new) {
		int id = new->id;
		struct kvm_memory_slot *old = id_to_memslot(slots, id);
		unsigned long npages = old->npages;

		*old = *new;
		if (new->npages != npages)
			sort_memslots(slots);
	}

	slots->generation = last_generation + 1;
}
