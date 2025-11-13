#!/usr/bin/env python3
import sys
import re
import os

def parse_pm_config(pm_config_path, partition_name):
    macros = {}
    pattern = re.compile(rf"#define\s+PM_{re.escape(partition_name.upper())}_(ADDRESS|SIZE|ID)\s+(\S+)")

    with open(pm_config_path, "r") as f:
        for line in f:
            m = pattern.match(line)
            if m:
                key = m.group(1)
                value = m.group(2)
                macros[key] = value

    if not all(k in macros for k in ("ADDRESS", "SIZE", "ID")):
        missing = [k for k in ("ADDRESS", "SIZE", "ID") if k not in macros]
        err_msg = f"Error: Partition '{partition_name}' missing macros: {', '.join(missing)} in {pm_config_path}"
        print(err_msg, file=sys.stderr)
        sys.exit(1)

    return macros


def append_nvs_fs_struct(c_file_path, partition_name, nvs_fs_name, macros):
    content = f"""
// Partition '{partition_name}' NVS FS definition
#define NVS_PARTITION {partition_name}
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(NVS_PARTITION)
#define NVS_PARTITION_SECTOR_SIZE 4096

struct nvs_fs {nvs_fs_name} = {{
    .flash_device = NVS_PARTITION_DEVICE,
//    .offset = PM_{partition_name.upper()}_ADDRESS,
    .offset = NVS_PARTITION_OFFSET,
    .sector_size = NVS_PARTITION_SECTOR_SIZE,
    .sector_count = PM_{partition_name.upper()}_SIZE / NVS_PARTITION_SECTOR_SIZE,
}};
"""

    with open(c_file_path, "a") as f:
        f.write(content)

    print(f"Appended NVS FS struct '{nvs_fs_name}' for partition '{partition_name}' to {c_file_path}")


def main():
    if len(sys.argv) != 5:
        print(f"Usage: {sys.argv[0]} <output_dir> <pm_config.h> <partition_name> <nvs_fs_name>", file=sys.stderr)
        sys.exit(1)

    output_dir = sys.argv[1]
    pm_config_path = sys.argv[2]
    partition_name = sys.argv[3]
    nvs_fs_name = sys.argv[4]

    c_file_path = os.path.join(output_dir, "lmt_ddt_fs.c")

    try:
        macros = parse_pm_config(pm_config_path, partition_name)
        append_nvs_fs_struct(c_file_path, partition_name, nvs_fs_name, macros)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
