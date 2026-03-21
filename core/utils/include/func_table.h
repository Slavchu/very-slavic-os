#pragma once

#include <stdint.h>

enum func_table_ids : uint16_t {
    FUNC_TABLE_PREINIT,
    FUNC_TABLE_INVALID /* Keep last */
};

struct func_table_entry {
    void (*cb)(void *);
    enum func_table_ids id;
};
#define FUNC_TABLE_APPEND(_id, _cb)                                                                \
    __attribute__((section(".func_tables." #_id),                                                  \
                   used)) static const struct func_table_entry func_table_entry_##_cb = {          \
        .id = _id,                                                                                  \
        .cb = _cb,                                                                                  \
    };

void run_table(const enum func_table_ids id, void *arg);
