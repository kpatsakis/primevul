validate_exec_list(struct drm_i915_gem_exec_object2 *exec,
		   int count)
{
	int i;

	for (i = 0; i < count; i++) {
		char __user *ptr = (char __user *)(uintptr_t)exec[i].relocs_ptr;
		int length; /* limited by fault_in_pages_readable() */

		/* First check for malicious input causing overflow */
		if (exec[i].relocation_count >
		    INT_MAX / sizeof(struct drm_i915_gem_relocation_entry))
			return -EINVAL;

		length = exec[i].relocation_count *
			sizeof(struct drm_i915_gem_relocation_entry);
		if (!access_ok(VERIFY_READ, ptr, length))
			return -EFAULT;

		/* we may also need to update the presumed offsets */
		if (!access_ok(VERIFY_WRITE, ptr, length))
			return -EFAULT;

		if (fault_in_pages_readable(ptr, length))
			return -EFAULT;
	}

	return 0;
}
