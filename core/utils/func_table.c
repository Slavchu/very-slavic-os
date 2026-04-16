#include <func_table.h>

void run_table(const enum func_table_ids id, void *arg) {
    extern struct func_table_entry *_func_tables_start;
    extern struct func_table_entry *_func_tables_end;

    for (struct func_table_entry *entry = _func_tables_start; entry < _func_tables_end; entry++) {
        if (entry->id == id) {
            entry->cb(arg);
        }
    }
}
