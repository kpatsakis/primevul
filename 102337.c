static int start_voice_call(struct audio_device *adev)
{
 struct audio_usecase *uc_info;

    ALOGV("%s: enter", __func__);

    uc_info = (struct audio_usecase *)calloc(1, sizeof(struct audio_usecase));
    uc_info->id = USECASE_VOICE_CALL;
    uc_info->type = VOICE_CALL;
    uc_info->stream = (struct audio_stream *)adev->primary_output;
    uc_info->devices = adev->primary_output->devices;
    uc_info->in_snd_device = SND_DEVICE_NONE;
    uc_info->out_snd_device = SND_DEVICE_NONE;

    uc_select_pcm_devices(uc_info);

    list_add_tail(&adev->usecase_list, &uc_info->adev_list_node);

    select_devices(adev, USECASE_VOICE_CALL);


 /* TODO: implement voice call start */

 /* set cached volume */
    set_voice_volume_l(adev, adev->voice_volume);

    adev->in_call = true;
    ALOGV("%s: exit", __func__);
 return 0;
}
