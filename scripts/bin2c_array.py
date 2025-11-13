#!/usr/bin/env python3

import sys
import os
import argparse

def to_c_array(data):
    lines = []
    for i in range(0, len(data), 12):
        chunk = data[i:i+12]
        line = ", ".join(f"0x{b:02X}" for b in chunk)
        lines.append("    " + line)
    return ",\n".join(lines)

def main():
    parser = argparse.ArgumentParser(description="Convert binary file to C array source and header files.")
    parser.add_argument("input_file", help="Path to input binary file")
    parser.add_argument("var_name", help="Name of the generated C array variable")
    parser.add_argument(
        "-o", "--output_dir",
        default=".",
        help="Output directory for generated .c and .h files (default: current directory)"
    )
    args = parser.parse_args()

    input_file = args.input_file
    var_name = args.var_name
    output_dir = args.output_dir

    # Ensure output directory exists
    os.makedirs(output_dir, exist_ok=True)

    # Read binary data
    with open(input_file, "rb") as f:
        data = f.read()

    # Compose filenames
    base_name = var_name + "_array"
    c_filename = os.path.join(output_dir, base_name + ".c")
    h_filename = os.path.join(output_dir, base_name + ".h")

    # Generate C array initializer
    array_data = to_c_array(data)
    array_size = len(data)

    # Write .c file
    with open(c_filename, "w") as c_file:
        c_file.write("#include <stdint.h>\n")
        c_file.write(f'#include "{base_name}.h"\n\n')
        c_file.write(f"const uint8_t {var_name}[{array_size}] = {{\n{array_data}\n}};\n")
        c_file.write(f"\nconst unsigned int {var_name}_len = {array_size};\n")

    # Write .h file
    with open(h_filename, "w") as h_file:
        guard = base_name.upper() + "_H"
        h_file.write(f"#ifndef {guard}\n")
        h_file.write(f"#define {guard}\n\n")
        h_file.write("#include <stdint.h>\n\n")
        h_file.write(f"extern const uint8_t {var_name}[];\n")
        h_file.write(f"extern const unsigned int {var_name}_len;\n")
        h_file.write("\n#endif\n")

    print(f"Generated {c_filename} and {h_filename}")

if __name__ == "__main__":
    main()
