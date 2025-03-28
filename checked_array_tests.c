#include "checked_array.h"
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct {
    uint8_t t1[sizeof(uintptr_t)];
} test_data;

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
        typedef CA_STRUCT_ONE(test_data) ca_one;
        ca_one t = CA_STRUCT_INIT();
        CA_ASSERT(t.start_canary, CA_CANARY);
        CA_ASSERT(t.end_canary, CA_CANARY);
        CA_ASSERT(CA_STRUCT_OK(t), true);
        ca_one *t_p = &t;
        CA_ASSERT(CA_STRUCT_OK(*t_p), true);

        
        t.item.t1[-1] = 'a';
        CA_ASSERT(CA_STRUCT_OK(t), false);
        CA_STRUCT_SETUP(t);
        CA_ASSERT(CA_STRUCT_OK(t), true);
        t.item.t1[sizeof(t.item.t1) + 1] = 'a';
        CA_ASSERT(CA_STRUCT_OK(t), false);
    }
    {
        typedef CA_STRUCT(test_data, 2) ca_two;
        ca_two t = CA_STRUCT_INIT();
        CA_ASSERT(t.start_canary, CA_CANARY);
        CA_ASSERT(t.end_canary, CA_CANARY);
        CA_ASSERT(CA_STRUCT_OK(t), true);

        CA_ASSERT(CA_STRUCT_I_OK(t, -1), false);
        CA_ASSERT(CA_STRUCT_I_OK(t, 0), true);
        CA_ASSERT(CA_STRUCT_I_OK(t, 1), true);
        CA_ASSERT(CA_STRUCT_I_OK(t, 2), false);

        ca_two *t_p = &t;
        CA_ASSERT(CA_STRUCT_OK(*t_p), true);

        t.data[0].t1[-1] = 'a';
        CA_ASSERT(CA_STRUCT_OK(t), false);
        CA_STRUCT_SETUP(t);
        CA_ASSERT(CA_STRUCT_OK(t), true);
        t.data[2].t1[0] = 'a';
        CA_ASSERT(CA_STRUCT_OK(t), false);
    }

    {
        typedef CA_LEN_STRUCT(test_data, 2) ca_two;
        ca_two t = CA_STRUCT_INIT();
        CA_ASSERT(t.start_canary, CA_CANARY);
        CA_ASSERT(t.end_canary, CA_CANARY);
        CA_ASSERT(CA_STRUCT_OK(t), true);

        CA_ASSERT(CA_STRUCT_I_OK(t, 0), true);
        CA_ASSERT(CA_STRUCT_I_OK(t, 1), true);
        CA_ASSERT(CA_STRUCT_I_OK(t, 2), false);

        ca_two *t_p = &t;
        CA_ASSERT(CA_STRUCT_OK(*t_p), true);

        t.data[0].t1[-1] = 'a';
        CA_ASSERT(CA_LEN_STRUCT_OK(t), false);
        CA_LEN_STRUCT_SETUP(t);
        CA_ASSERT(CA_LEN_STRUCT_OK(t), true);
        t.data[2].t1[0] = 'a';
        CA_ASSERT(CA_LEN_STRUCT_OK(t), false);
    }
    {
        uintptr_t test_mem[3] = {0};
        CA_ASSERT(ca_init_mem(test_mem, sizeof(test_mem), NULL), CA_OK);
        CA_ASSERT(ca_check_mem(test_mem, sizeof(test_mem)), CA_OK);
        test_mem[0] = 0;
        CA_ASSERT(ca_check_mem(test_mem, sizeof(test_mem)), CA_ERR);

        void *mem_start = NULL;
        CA_ASSERT(ca_init_mem(test_mem, sizeof(test_mem), &mem_start), CA_OK);
        CA_ASSERT((uintptr_t)mem_start, (uintptr_t)&test_mem[1]);
        CA_ASSERT(ca_check_mem(test_mem, sizeof(test_mem)), CA_OK);
        test_mem[2] = 0;
        CA_ASSERT(ca_check_mem(test_mem, sizeof(test_mem)), CA_ERR);

        // Try with a different elignment to make sure it still works.
        uint8_t *test_mem8 = ((uint8_t*)test_mem) + 1;
        uint32_t size8 = sizeof(test_mem) - 1;
        CA_ASSERT(ca_init_mem(test_mem8, size8, NULL), CA_OK);
        CA_ASSERT(ca_check_mem(test_mem8, size8), CA_OK);
        test_mem8[0] = 3;
        CA_ASSERT(ca_check_mem(test_mem8, size8), CA_ERR);

        CA_ASSERT(ca_init_mem(test_mem8, size8, NULL), CA_OK);
        test_mem8[size8 - 1] = 4;
        CA_ASSERT(ca_check_mem(test_mem8, size8), CA_ERR);
    }
    printf("Yipee! All tests passed!\n");
    return 0;
}
