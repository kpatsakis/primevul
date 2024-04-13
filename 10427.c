static int _snd_pcm_hw_param_min(struct snd_pcm_hw_params *params,
				 snd_pcm_hw_param_t var, unsigned int val,
				 int dir)
{
	int changed;
	int open = 0;
	if (dir) {
		if (dir > 0) {
			open = 1;
		} else if (dir < 0) {
			if (val > 0) {
				open = 1;
				val--;
			}
		}
	}
	if (hw_is_mask(var))
		changed = snd_mask_refine_min(hw_param_mask(params, var),
					      val + !!open);
	else if (hw_is_interval(var))
		changed = snd_interval_refine_min(hw_param_interval(params, var),
						  val, open);
	else
		return -EINVAL;
	if (changed > 0) {
		params->cmask |= 1 << var;
		params->rmask |= 1 << var;
	}
	return changed;
}