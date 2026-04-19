#include <func_table.h>
#include <log.h>
#include <stdint.h>

void run_table(const enum func_table_ids id, void *arg) {
    extern struct func_table_entry _func_tables_start[];
    extern struct func_table_entry _func_tables_end[];

    uintptr_t end_addr = (uintptr_t)_func_tables_end;

    for (struct func_table_entry *entry = _func_tables_start; (uintptr_t)entry < end_addr; entry++) {
        if (entry->id == id && entry->cb) {
            entry->cb(arg);
        }
    }
}
