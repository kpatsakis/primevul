int sev_es_string_io(struct vcpu_svm *svm, int size, unsigned int port, int in)
{
	if (!setup_vmgexit_scratch(svm, in, svm->vmcb->control.exit_info_2))
		return -EINVAL;

	return kvm_sev_es_string_io(&svm->vcpu, size, port,
				    svm->ghcb_sa, svm->ghcb_sa_len / size, in);
}