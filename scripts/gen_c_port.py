#!/usr/bin/env python3

# Copyright 2026 Latvijas Mobilais Telefons
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

import os
import argparse

HEADER_TEMPLATE = """#ifndef {guard}
#define {guard}

#include <zephyr/kernel.h>

extern struct k_thread *p_logger_thread_data;
extern struct k_thread *p_packer_thread_data;
extern struct k_thread *p_mailer_thread_data;

#endif // {guard}
"""

# {weak} is "__weak " for the library build and "" for an application build.
# The object is allocated in both, so the library-building application links
# and boots, while a target application overrides the pointer with its own.
SOURCE_TEMPLATE = """#include <zephyr/kernel.h>
#include <zephyr/toolchain.h>
#include "{name}.h"

static struct k_thread logger_thread_data;
static struct k_thread packer_thread_data;
static struct k_thread mailer_thread_data;

{weak}struct k_thread *p_logger_thread_data = &logger_thread_data;
{weak}struct k_thread *p_packer_thread_data = &packer_thread_data;
{weak}struct k_thread *p_mailer_thread_data = &mailer_thread_data;
"""


def main():
    parser = argparse.ArgumentParser(
        description="Generate the kernel object port C files."
    )
    parser.add_argument(
        "-o", "--output_dir",
        default=".",
        help="Output directory (default: current directory)"
    )
    parser.add_argument(
        "-n", "--name",
        default="gen_c_port",
        help="Base name for generated files (default: gen_c_port)"
    )
    parser.add_argument(
        "-w", "--weak",
        action="store_true",
        help="Generate the library definitions with weak linkage (default: strong, for app builds)"
    )

    args = parser.parse_args()

    output_dir = args.output_dir
    name = args.name

    os.makedirs(output_dir, exist_ok=True)

    guard = name.upper() + "_H"

    h_path = os.path.join(output_dir, f"{name}.h")
    c_path = os.path.join(output_dir, f"{name}.c")

    # Write header
    with open(h_path, "w") as f:
        f.write(HEADER_TEMPLATE.format(
            guard=guard
        ))

    # Write source
    with open(c_path, "w") as f:
        f.write(SOURCE_TEMPLATE.format(
            name=name,
            weak="__weak " if args.weak else ""
        ))

    print(f"Generated {h_path} and {c_path}")


if __name__ == "__main__":
    main()
