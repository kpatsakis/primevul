static void v9fs_init_qiov_from_pdu(QEMUIOVector *qiov, V9fsPDU *pdu,
                                    size_t skip, size_t size,
                                    bool is_write)
{
    QEMUIOVector elem;
    struct iovec *iov;
    unsigned int niov;

    virtio_init_iov_from_pdu(pdu, &iov, &niov, is_write);

    qemu_iovec_init_external(&elem, iov, niov);
    qemu_iovec_init(qiov, niov);
    qemu_iovec_concat(qiov, &elem, skip, size);
}
