#ifndef __SENDFD_H_A10E0BAC9AA4C142ABC15731138AB3C3__
#define __SENDFD_H_A10E0BAC9AA4C142ABC15731138AB3C3__
/******************************************************************************/
#include "utils.h"
#include "fd/fd.h"

#ifndef FDSENDER_UNIX_SOCKET
#define FDSENDER_UNIX_SOCKET    "/var/run/fdsender.sock"
#endif

static inline int //return fd
os_fdtransfer(void)
{
    int fd = INVALID_COMMON_ID;
    int err;
    struct sockaddr_un local = OS_SOCKADDR_UNIX(FDSENDER_UNIX_SOCKET);
    
    fd = os_socket(PF_UNIX, SOCK_DGRAM, 0);
    if (fd<0) {
        return errno;
    }

    err = os_bind(fd, (struct sockaddr*)&local, sizeof(local));
    if (err<0) {
        close(fd);

        return errno;
    }
    
    return fd;
}

static inline int
os_fdtransfer_close(int fd)
{
    return os_close(fd);
}

#define __SENDFD_CMSGSIZE (sizeof(struct cmsghdr) + sizeof(int))

#define __FDTRANSFER(_buf, _bufcmptr, _cmptr, _iov, _msg) \
    char _buf[2] = {0}; \
    char _bufcmptr[__SENDFD_CMSGSIZE] = {0}; \
    struct cmsghdr *_cmptr = (struct cmsghdr *)_bufcmptr; \
    struct iovec _iov = OS_IOVEC_INITER(_buf, sizeof(_buf)); \
    struct msghdr _msg = OS_MSGHDR_INITER(&_iov, 1, NULL, 0, _cmptr, __SENDFD_CMSGSIZE)

static inline int
os_fdsend(int sender, int fd)
{
    int err = 0;
    __FDTRANSFER(buf, bufcmptr, cmptr, iov, msg);
    
    if (false==is_good_fd(sender)) {
        return os_assert_value(-EINVAL1);
    }
    if (false==is_good_fd(fd)) {
        return os_assert_value(-EINVAL2);
    }

    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type  = SCM_RIGHTS;
    cmptr->cmsg_len   = __SENDFD_CMSGSIZE;
    *(int *)CMSG_DATA(cmptr) = fd;

    err = sendmsg(sender, &msg, 0);
    if (2!=err) {
        return errno;
    }
  
    return 0;  
}


static inline int
os_fdrecv(int recver)
{
    int err = 0;
    __FDTRANSFER(buf, bufcmptr, cmptr, iov, msg);
    
    if (false==is_good_fd(recver)) {
        return os_assert_value(-EINVAL1);
    }
    
    err = recvmsg(recver, &msg, 0);
    if (2!=err) {
        return errno;
    }
    
    return *(int*)CMSG_DATA(cmptr);
}


/******************************************************************************/
#endif /* __SENDFD_H_A10E0BAC9AA4C142ABC15731138AB3C3__ */
