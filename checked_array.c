#include "checked_array.h"

#include <stdbool.h>
#include <string.h>

struct {
    uintptr_t first,
              second;
} size_struct;
ca_err ca_init_mem(void *mem, uintptr_t mem_len) {
    if (sizeof(size_struct) <= mem_len) {
        return CA_BUF_OVF;
    }
    uintptr_t canary = CA_CANARY;
    if (((uintptr_t)mem % sizeof(canary)) == 0) {
        *(uintptr_t*)mem = canary;
    } else {
        memcpy(mem, &canary, sizeof(canary));
    }
    
    uint8_t *end_canary_ptr = (uint8_t*)mem + mem_len - sizeof(canary);
    if (((uintptr_t)end_canary_ptr % sizeof(canary)) == 0) {
        *(uintptr_t*)end_canary_ptr = canary;
    } else {
        memcpy(end_canary_ptr, &canary, sizeof(canary));
    }
    return CA_OK;
}

ca_err ca_check_mem(void* mem, uintptr_t mem_len) {
    if (sizeof(size_struct) <= mem_len) {
        return CA_BUF_OVF;
    }
    uintptr_t canary = CA_CANARY;
    if (((uintptr_t)mem % sizeof(canary)) == 0) {
        if (*(uintptr_t*)mem != canary) {
            return CA_ERR;
        }
    } else {
        if (memcmp(mem, &canary, sizeof(canary)) != 0) {
            return CA_ERR;
        }
    }
    
    uint8_t *end_canary_ptr = (uint8_t*)mem + mem_len - sizeof(canary);
    if (((uintptr_t)end_canary_ptr % sizeof(canary)) == 0) {
        if (*(uintptr_t*)end_canary_ptr != canary) {
            return CA_ERR;
        }
    } else {
        if (memcmp(end_canary_ptr, &canary, sizeof(canary)) != 0) {
            return CA_ERR;
        }
    }
    return CA_OK;
}
