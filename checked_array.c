#include "checked_array.h"

#include <stdbool.h>
#include <string.h>

struct {
    uintptr_t first,
              second;
} ca_size_struct;

static void ca_align_init_mem(void *mem, uintptr_t canary) {
    if (((uintptr_t)mem % sizeof(canary)) == 0) {
        *(uintptr_t*)mem = canary;
    } else {
        memcpy(mem, &canary, sizeof(canary));
    }
}

ca_err ca_init_mem(void *mem, uintptr_t mem_len, void **mem_start) {
    if (sizeof(ca_size_struct) <= mem_len) {
        return CA_BUF_OVF;
    }
    const uintptr_t canary = CA_CANARY;
    ca_align_init_mem(mem, canary);
    
    uint8_t *end_canary_ptr = (uint8_t*)mem + mem_len - sizeof(canary);
    ca_align_init_mem(end_canary_ptr, canary);
    if (mem_start != NULL) {
        *mem_start = (uint8_t*)mem + sizeof(canary);
    }
    return CA_OK;
}

static ca_err ca_align_check_mem(const void* mem, uintptr_t canary) {
    if (((uintptr_t)mem % sizeof(canary)) == 0) {
        if (*(uintptr_t*)mem != canary) {
            return CA_ERR;
        }
    } else if (memcmp(mem, &canary, sizeof(canary)) != 0) {
        return CA_ERR;
    }
    return CA_OK;
}

ca_err ca_check_mem(const void* mem, uintptr_t mem_len) {
    if (sizeof(ca_size_struct) <= mem_len) {
        return CA_BUF_OVF;
    }
    const uintptr_t canary = CA_CANARY;
    uint8_t *end_canary_ptr = (uint8_t*)mem + mem_len - sizeof(canary);
    ca_err r = CA_OK;
    if (CA_OK != (r = ca_align_check_mem(mem, canary)) ||
        CA_OK != (r = ca_align_check_mem(end_canary_ptr, canary))
       ) {}
    return r;
}
