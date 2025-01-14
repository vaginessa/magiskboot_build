// long variants

extern "C" {
    #[link_name = "lseek"]
    pub fn lsee64(fd: crate::c_int, offset: crate::off_t, whence: crate::c_int) -> crate::off_t;
}

// syscall

pub const SYS_dup3: crate::c_int = 0;

// sendfile

extern "C" {
    #[link_name = "_sendfile_stub"]
    fn real_sendfile(
        out_fd: crate::c_int,
        in_fd: crate::c_int,
        count: crate::size_t
    );
}

f! {
    pub fn sendfile(
        out_fd: crate::c_int,
        in_fd: crate::c_int,
        offset: *mut crate::off_t,
        count: crate::size_t
    ) -> crate::ssize_t {
        if offset.is_null() {
            real_sendfile(out_fd, in_fd, count);
            0
        else {
            panic!("unreachable code")
        }
    }
}
