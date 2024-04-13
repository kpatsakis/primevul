static int vhost_net_enable_vq(struct vhost_net *n,
				struct vhost_virtqueue *vq)
{
	struct vhost_net_virtqueue *nvq =
		container_of(vq, struct vhost_net_virtqueue, vq);
	struct vhost_poll *poll = n->poll + (nvq - n->vqs);
	struct socket *sock;

	sock = rcu_dereference_protected(vq->private_data,
					 lockdep_is_held(&vq->mutex));
	if (!sock)
		return 0;

	return vhost_poll_start(poll, sock->file);
}
