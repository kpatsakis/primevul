static ssize_t snd_pcm_oss_write2(struct snd_pcm_substream *substream, const char *buf, size_t bytes, int in_kernel)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	snd_pcm_sframes_t frames, frames1;
#ifdef CONFIG_SND_PCM_OSS_PLUGINS
	if (runtime->oss.plugin_first) {
		struct snd_pcm_plugin_channel *channels;
		size_t oss_frame_bytes = (runtime->oss.plugin_first->src_width * runtime->oss.plugin_first->src_format.channels) / 8;
		if (!in_kernel) {
			if (copy_from_user(runtime->oss.buffer, (const char __force __user *)buf, bytes))
				return -EFAULT;
			buf = runtime->oss.buffer;
		}
		frames = bytes / oss_frame_bytes;
		frames1 = snd_pcm_plug_client_channels_buf(substream, (char *)buf, frames, &channels);
		if (frames1 < 0)
			return frames1;
		frames1 = snd_pcm_plug_write_transfer(substream, channels, frames1);
		if (frames1 <= 0)
			return frames1;
		bytes = frames1 * oss_frame_bytes;
	} else
#endif
	{
		frames = bytes_to_frames(runtime, bytes);
		frames1 = snd_pcm_oss_write3(substream, buf, frames, in_kernel);
		if (frames1 <= 0)
			return frames1;
		bytes = frames_to_bytes(runtime, frames1);
	}
	return bytes;
}