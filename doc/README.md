# Documentation Build Guide

This directory contains the documentation source and build configuration for the LMT IoT Shortcut SDK documentation.

## Build Process

Documentation is built using **Doxygen** with manual **.tex file edits** for customization.

### Building the Documentation

1. Run Doxygen with the configuration file:
   ```
   doxygen doxy_config
   ```

2. The LaTeX intermediate files will be generated in `./latex/`

3. Edit the `.tex` files as needed for manual customizations

4. Compile the PDF from the LaTeX output

## Output

The final documentation is generated as `LMT IoT Shortcut SDK API.pdf`.
