#!/usr/bin/env python3

# Copyright 2025 Latvijas Mobilais Telefons
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
