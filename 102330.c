static int adev_set_mic_mute(struct audio_hw_device *dev, bool state)
{
 struct audio_device *adev = (struct audio_device *)dev;
 int err = 0;

    pthread_mutex_lock(&adev->lock);
    adev->mic_mute = state;

 if (adev->mode == AUDIO_MODE_IN_CALL) {
 /* TODO */
 }

    pthread_mutex_unlock(&adev->lock);
 return err;
}
