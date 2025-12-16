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

import argparse
import os
import re
import sys

def find_partition_macros(pm_config_path, partition_name):
    """Search for PM_<PARTITION>_ID, _ADDRESS, _SIZE in pm_config.h"""
    macros = {}
    prefix = f"PM_{re.escape(partition_name.upper())}_"
    pattern = re.compile(rf"#define\s+({prefix}(ID|ADDRESS|SIZE))\s+(\S+)")

    with open(pm_config_path, "r") as f:
        for line in f:
            match = pattern.match(line)
            if match:
                macros[match.group(2)] = match.group(3)

    missing = [k for k in ("ID", "ADDRESS", "SIZE") if k not in macros]
    if missing:
        raise ValueError(f"Missing in pm_config.h for partition '{partition_name}': {', '.join(missing)}")

    return macros


def generate_code(out_dir, partition_name, config_var, mount_var, mount_point):
    c_file = os.path.join(out_dir, "lmt_ddt_fs.c")
    os.makedirs(out_dir, exist_ok=True)

    content = f"""
// Partition '{partition_name}' LittleFS config
FS_LITTLEFS_DECLARE_DEFAULT_CONFIG({config_var});

struct fs_mount_t {mount_var} = {{
    .type = FS_LITTLEFS,
    .fs_data = &{config_var},
    .storage_dev = (void *)FIXED_PARTITION_ID({partition_name}),
    .mnt_point = "{mount_point}",
}};
"""

    with open(c_file, "a") as f:
        f.write(content)

    print(f"Appended LittleFS structs for partition '{partition_name}' to {c_file}")


def main():
    parser = argparse.ArgumentParser(description="Generate LittleFS config C structs for Zephyr")
    parser.add_argument("output_dir", help="Directory where lmt_ddt_fs.c is generated")
    parser.add_argument("pm_config_h", help="Path to generated pm_config.h")
    parser.add_argument("partition", help="Partition name (e.g., littlefs_storage)")
    parser.add_argument("config_var", help="C variable name for fs_littlefs struct")
    parser.add_argument("mount_var", help="C variable name for fs_mount_t struct")
    parser.add_argument("mount_point", help="Filesystem mount point (e.g., /lfs)")

    args = parser.parse_args()

    try:
        _ = find_partition_macros(args.pm_config_h, args.partition)
    except ValueError as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

    generate_code(args.output_dir, args.partition, args.config_var, args.mount_var, args.mount_point)


if __name__ == "__main__":
    main()
