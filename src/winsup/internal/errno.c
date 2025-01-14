#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winternl.h>

#include "winerr_map.h"

#ifndef NDEBUG
#include "assert.h"

#define LOG_TAG             "errno_internal"
#define LOG_ERR(...)        log_err(LOG_TAG, __VA_ARGS__);
#endif

void __set_errno_via_winerr(DWORD winerr) {
    if (winerr == NO_ERROR)
        errno = 0;
    else if (winerr < __winerr_map_size) {
        int mapped_err = __winerr_map[winerr];

        if (!mapped_err)
            goto unk_err;

        errno = mapped_err;
    } else {
unk_err:
#ifndef NDEBUG
        LOG_ERR("Unmapped winerr: %s (%ld)", win_strerror(winerr), winerr)
#endif

        errno = -1;
    }

    return;
}

void __set_errno_via_ntstatus(DWORD status) {
    if (NT_SUCCESS(status))
        errno = 0;
    else {
        DWORD winerr = RtlNtStatusToDosError(status);

        if (winerr == ERROR_MR_MID_NOT_FOUND) {
#ifndef NDEBUG
            LOG_ERR("Unmapped ntstatus: %s (%ld)", nt_strstatus(status), status)
#endif
            errno = -1;
        } else
            __set_errno_via_winerr(winerr);
    }

    return;
}
