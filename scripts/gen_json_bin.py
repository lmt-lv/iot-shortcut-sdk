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
import json
import argparse
import sys
import re

# Required fields for COAP.json - add/modify as needed
REQUIRED_COAP_FIELDS = [
    "SERVER_HOSTNAME",
    "SERVER_PORT",
    "COAP_TX_RESOURCE",
    "COAP_TX_FILE_RESOURCE",
    "COAP_TX_FW_RESOURCE"
]

def validate_coap_json_duplicates(file_path):
    """Check for duplicate keys in JSON file by parsing manually"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Find all key definitions (handles both "key" and 'key')
        key_pattern = r'["\']([^"\']+)["\']\s*:'
        keys_found = re.findall(key_pattern, content)
        
        # Check for duplicates
        seen_keys = set()
        duplicate_keys = []
        
        for key in keys_found:
            if key in seen_keys:
                duplicate_keys.append(key)
            else:
                seen_keys.add(key)
        
        if duplicate_keys:
            print(f"Error: Duplicate keys found in COAP.json: {', '.join(set(duplicate_keys))}")
            return False
            
        return True
        
    except Exception as e:
        print(f"Error checking for duplicates in COAP.json: {e}")
        return False

def validate_coap_json(coap_data, filename):
    """Validate that all required COAP fields are present and have valid values"""
    missing_fields = []
    invalid_fields = []
    
    for field in REQUIRED_COAP_FIELDS:
        if field not in coap_data:
            missing_fields.append(field)
        else:
            value = coap_data[field]
            
            # Special validation for port (must be positive integer)
            if field == "SERVER_PORT":
                # Try to convert to int if it's a string representation
                try:
                    port_value = int(value)
                    if port_value < 1 or port_value > 65535:
                        invalid_fields.append(f"{field} (must be valid port 1-65535, got: {value})")
                except (ValueError, TypeError):
                    invalid_fields.append(f"{field} (must be a valid integer port, got: {value})")
            # String fields must not be empty/whitespace
            else:
                if not isinstance(value, str) or not value.strip():
                    invalid_fields.append(f"{field} (empty or invalid string)")
    
    if missing_fields:
        print(f"Error: Missing required fields in {filename}: {', '.join(missing_fields)}")
        return False
        
    if invalid_fields:
        print(f"Error: Invalid field values in {filename}: {', '.join(invalid_fields)}")
        return False
        
    return True

def write_json_pairs_as_c_strings(input_dir, output_file, max_size):
    filenames = ["COAP.json", "Settings.json", "Application.json"]
    c_strings = bytearray()
    coap_validated = False
    all_keys = {}  # Track keys across all files
    duplicate_keys = []

    for fname in filenames:
        path = os.path.join(input_dir, fname)
        if os.path.isfile(path):
            print(f"File: {path}")
            
            # Special duplicate check for COAP.json
            if fname == "COAP.json":
                if not validate_coap_json_duplicates(path):
                    sys.exit(1)
            
            with open(path, "r", encoding="utf-8") as f:
                try:
                    data = json.load(f)
                    
                    # Special validation for COAP.json
                    if fname == "COAP.json":
                        if not validate_coap_json(data, fname):
                            sys.exit(1)
                        coap_validated = True
                        print(f"COAP.json validation PASSED - all required fields present and no duplicates")
                    
                    # Check for duplicate keys across files
                    for key in data.keys():
                        if key in all_keys:
                            duplicate_keys.append(f"'{key}' found in both {all_keys[key]} and {fname}")
                        else:
                            all_keys[key] = fname
                    
                    for key, value in data.items():
                        key_bytes = str(key).encode('utf-8') + b'\0'
                        value_bytes = str(value).encode('utf-8') + b'\0'

                        if len(c_strings) + len(key_bytes) + len(value_bytes) + 1 > max_size:
                            print(f"Error: Adding '{key}' would exceed {max_size} bytes. Aborting.")
                            sys.exit(1)

                        c_strings += key_bytes + value_bytes

                except json.JSONDecodeError as e:
                    print(f"Skipping {fname}: Invalid JSON. Error: {e}")
                    if fname == "COAP.json":
                        print(f"Error: COAP.json is required and must be valid JSON!")
                        sys.exit(1)
                    continue
        else:
            # Check if missing file is COAP.json (required)
            if fname == "COAP.json":
                print(f"Error: Required file {fname} not found in {input_dir}")
                sys.exit(1)
            else:
                print(f"Optional file {fname} not found - skipping")

    # Check for duplicate keys across files
    if duplicate_keys:
        print(f"Error: Duplicate keys found across JSON files:")
        for duplicate in duplicate_keys:
            print(f"  - {duplicate}")
        sys.exit(1)

    # Ensure COAP.json was processed and validated
    if not coap_validated:
        print(f"Error: COAP.json was not found or validated!")
        sys.exit(1)

    # Final null byte
    if len(c_strings) + 1 > max_size:
        print(f"Error: Final null byte would exceed {max_size} bytes. Aborting.")
        sys.exit(1)

    c_strings += b'\0'

    try:
        with open(output_file, "wb") as out:
            out.write(c_strings)
    except Exception as e:
        print(f"Error writing to output file: {e}")
        sys.exit(1)

    if ( 1 == len(c_strings) ):
        print(f"No valid configuration settings in /etc/*.json!")
        if os.environ.get("ZEPHYR_IN_DOCKER") != "1":
            sys.exit(2)
        
    print(f"Successfully written to {output_file} ({len(c_strings)} bytes).")
    print(f"Processed {len(all_keys)} unique keys from {len([f for f in filenames if os.path.isfile(os.path.join(input_dir, f))])} files.")
    sys.exit(0)

def main():
    parser = argparse.ArgumentParser(
        description="Convert JSON key/value pairs from COAP, Settings, and Application files into a binary file with C strings (key\\0value\\0...)."
    )
    parser.add_argument(
        "--input-dir",
        required=True,
        help="Directory containing COAP.json, Settings.json, and Application.json files."
    )
    parser.add_argument(
        "--output-file",
        required=True,
        help="Path to output binary file."
    )
    parser.add_argument(
        "--max-size",
        type=int,
        required=True,
        help="Maximum allowed size of the output file in bytes."
    )

    args = parser.parse_args()
    write_json_pairs_as_c_strings(args.input_dir, args.output_file, args.max_size)

if __name__ == "__main__":
    main()