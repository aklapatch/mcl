#pragma once

#include <stdint.h>

#if UINTPTR_MAX == UINT64_MAX
#define CA_CANARY (0x000aff0d000aff0d)
#elif UINTPTR_MAX == UINT32_MAX
#define CA_CANARY (0x000aff0d)
#else
#error Missing canary for this pointer size!
#endif

typedef enum {
    CA_OK = 0,
    CA_ERR,
    CA_BUF_OVF,
    CA_NUM_ERRS,
} ca_err;

#define CA_STRUCT_ONE(type) \
    struct { \
        uintptr_t start_canary; \
        type item; \
        uintptr_t end_canary; \
    }

#define CA_STRUCT(type, len)\
    struct { \
        uintptr_t start_canary; \
        type data[(len)]; \
        uintptr_t end_canary; \
    }

#define CA_STRUCT_INIT() { .start_canary = CA_CANARY, .end_canary = CA_CANARY }

#define CA_STRUCT_SETUP(ca_struct) \
    do {\
        ca_struct.start_canary = CA_CANARY; \
        ca_struct.end_canary = CA_CANARY;\
    } while (0)

#define CA_STRUCT_OK(in) ((in).start_canary == CA_CANARY && (in).end_canary == CA_CANARY)

#define CA_STRUCT_I_OK(in, i) (CA_STRUCT_OK(in) && i < sizeof((in).data) / sizeof((in).data[0]))

#define CA_LEN_STRUCT(type, arr_len)\
    struct { \
        uintptr_t start_canary, \
                  len, \
                  mid_canary; \
        type data[(arr_len)]; \
        uintptr_t end_canary; \
    }

#define CA_LEN_STRUCT_INIT() {.start_canary = CA_CANARY, .mid_canary = CA_CANARY, .end_canary = CA_CANARY}

#define CA_LEN_STRUCT_OK(in) (CA_STRUCT_OK(in) && (in).mid_canary == CA_CANARY)

#define CA_LEN_STRUCT_I_OK(in) (CA_LEN_STRUCT_OK(in) && i < sizeof((in).data))

ca_err ca_init_mem(void *mem, uintptr_t mem_len, void **mem_start);

ca_err ca_check_mem(const void* mem, uintptr_t mem_len);
