#include "checked_array.h"
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct {
    uint8_t t1[sizeof(uintptr_t)];
} test_data;

typedef CA_STRUCT_ONE(test_data) ca_one;

typedef CA_STRUCT(test_data, 2) ca_two;

typedef CA_LEN_STRUCT(test_data, 2) ca_len;

#define CA_ASSERT(act, exp)\
    do {\
        uintptr_t _tmp_act = act,\
                  _tmp_exp = exp;\
        if (_tmp_act != _tmp_exp) {\
            printf("%s:%u Expected %" PRIxPTR " (%s), but got %" PRIxPTR " (%s)!\n", __func__, __LINE__, _tmp_exp, #exp, _tmp_act, #act);\
            return 1;\
        } \
    } while (0)


int main(void) {
    {
        ca_one t = CA_STRUCT_INIT();
        CA_ASSERT(t.start_canary, CA_CANARY);
        CA_ASSERT(t.end_canary, CA_CANARY);
        CA_ASSERT(CA_STRUCT_OK(t), true);
        ca_one *t_p = &t;
        CA_ASSERT(CA_STRUCT_OK(*t_p), true);

        
        // Overwrite the first canary.
        t.item.t1[-1] = 'a';
        CA_ASSERT(CA_STRUCT_OK(t), false);
        CA_STRUCT_SETUP(t);
        CA_ASSERT(CA_STRUCT_OK(t), true);
        t.item.t1[sizeof(t.item.t1) + 1] = 'a';
        CA_ASSERT(CA_STRUCT_OK(t), false);

        CA_STRUCT_SETUP(t);
        CA_ASSERT(CA_STRUCT_OK(t), true);
    }

    printf("Yipee! All tests passed!\n");
    return 0;
}
