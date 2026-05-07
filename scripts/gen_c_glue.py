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

import os
import argparse

HEADER_TEMPLATE = """#ifndef {guard}
#define {guard}

#include <stdint.h>

#if defined(CONFIG_COAP_QUEUE_SIZE)
#define MAX_COUNT_OF_COAP_REQUESTS CONFIG_COAP_QUEUE_SIZE
#else
#define MAX_COUNT_OF_COAP_REQUESTS 96
#endif

extern uint8_t {name}[];
extern const unsigned int {name}_size;

#endif // {guard}
"""

SOURCE_TEMPLATE = """#include <stdint.h>
#include "{name}.h"
#include "lmt_coap_manager.h"

uint8_t {name}[MAX_COUNT_OF_COAP_REQUESTS * APP_COAP_MAX_MSG_LEN];
const unsigned int {name}_size = MAX_COUNT_OF_COAP_REQUESTS * APP_COAP_MAX_MSG_LEN;
"""

def main():
    parser = argparse.ArgumentParser(
        description="Generate CoAP queue glue C files."
    )
    parser.add_argument(
        "-o", "--output_dir",
        default=".",
        help="Output directory (default: current directory)"
    )
    parser.add_argument(
        "-n", "--name",
        default="coap_queue",
        help="Base name for generated files (default: coap_queue)"
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
            guard=guard,
            name=name
        ))

    # Write source
    with open(c_path, "w") as f:
        f.write(SOURCE_TEMPLATE.format(
            name=name
        ))

    print(f"Generated {h_path} and {c_path}")

if __name__ == "__main__":
    main()
