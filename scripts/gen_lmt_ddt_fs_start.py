#!/usr/bin/env python3
import sys
import os

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <output_dir>", file=sys.stderr)
        sys.exit(1)

    output_dir = sys.argv[1]
    c_file = os.path.join(output_dir, "lmt_ddt_fs.c")

    content = """\
/*
 * LMT DDT FS definitions
 */

#include <zephyr/fs/fs.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/fs/littlefs.h>

"""

    os.makedirs(output_dir, exist_ok=True)
    with open(c_file, "w") as f:
        f.write(content)

    print(f"Generated start of C file: {c_file}")

if __name__ == "__main__":
    main()
