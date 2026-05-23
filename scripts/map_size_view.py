#!/usr/bin/env python3
import re
import sys
from pathlib import Path


def parse_map_sizes(file_path):
    path = Path(file_path)

    if not path.is_file():
        print(f"Error: File '{file_path}' does not exist.", file=sys.stderr)
        sys.exit(1)

    # Regex for standard single line (Section, Address, Size)
    single_line_re = re.compile(
        r"^\s+(\.[a-zA-Z0-9_\.-]+)\s+(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)(?:\s+.*)?$"
    )
    
    # Regex for a wrapped section name (Linker puts name on line 1, data on line 2)
    wrap_name_re = re.compile(r"^\s+(\.[a-zA-Z0-9_\.-]+)$")
    # Regex for the data portion of a wrapped line
    wrap_data_re = re.compile(r"^\s+(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)(?:\s+.*)?$")

    entries = []
    pending_section = None

    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            # Check for a standard single line
            match = single_line_re.match(line)
            if match:
                raw_section = match.group(1)
                size_hex = match.group(3)
                add_entry(entries, raw_section, size_hex)
                pending_section = None
                continue

            # Check for the start of a wrapped line
            match_wrap_name = wrap_name_re.match(line)
            if match_wrap_name:
                pending_section = match_wrap_name.group(1)
                continue

            # Check for the second half of a wrapped line
            if pending_section:
                match_wrap_data = wrap_data_re.match(line)
                if match_wrap_data:
                    size_hex = match_wrap_data.group(2)
                    add_entry(entries, pending_section, size_hex)
                # Reset pending state whether it matched or not
                pending_section = None
                
    return entries


def add_entry(entries, raw_section, size_hex):
    try:
        size_bytes = int(size_hex, 16)
    except ValueError:
        return

    # Ignore 0-byte allocations
    if size_bytes == 0:
        return

    # Parse ".text.my_func" into section ".text" and name "my_func"
    parts = raw_section.split(".", 2)
    
    if len(parts) > 2:
        section = f".{parts[1]}"
        name = parts[2]
    elif len(parts) == 2:
        section = f".{parts[1]}"
        name = "[Entire Section Block]"
    else:
        section = raw_section
        name = "[Unknown]"

    entries.append({"size": size_bytes, "name": name, "section": section})


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 map_size_view.py <path_to_gcc_map_file>", file=sys.stderr)
        sys.exit(1)

    # Fetch the parsed and populated entries
    entries = parse_map_sizes(sys.argv[1])

    if not entries:
        print("No valid sized sections found.", file=sys.stderr)
        sys.exit(1)

    # Sort from biggest to lowest
    entries.sort(key=lambda x: x["size"], reverse=True)

    # Header
    print(f"{'SIZE':<10} | {'NAME':<50} | {'SECTION'}")
    print("-" * 80)

    # Print rows
    for e in entries:
        print(f"{e['size']:<10} | {e['name']:<50} | {e['section']}")
