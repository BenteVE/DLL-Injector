# DLL Injector for Windows (x86/x64)

![DLL Injector in Action](placeholder_image_url.png)

## Overview

This repository contains a simple DLL (Dynamic Link Library) injector designed for Windows programs, supporting both x86 and x64 architectures. The injector allows you to inject custom DLLs into running processes, enabling you to modify their behavior dynamically.

## Features

- **Cross-Architecture Support:** Works seamlessly with both x86 and x64 Windows programs.
- **Example DLLs:** Included are two sample DLLs that you can use to test the injector.
- **Visual Studio Project:** The injector is developed using Microsoft Visual Studio for easy compilation and modification.

## Usage

1. **Build the Injector:**
   - Open the `DLLInjector.sln` solution file in Visual Studio.
   - Build the solution for the desired architecture (x86 or x64).
   - Note: the injector and the target program should have the same architecture!

2. **Prepare Target Process:**
   - Run the target application you want to inject the DLL into.

3. **Configure the conf.ini file:**
   - Set the path to the DLL
   - Identify the target process using the PID, the application name, or the window title

4. **Run the Injector:**
   - Launch the built injector executable (`DLLInjector.exe`).

5. **Verify Results:**
   - Observe the console output or any log messages to confirm successful injection.

## Examples

### Example DLLs

This repository includes two example DLLs:

1. **ExampleDLL1.dll:**
   - Illustrates a basic injected DLL with minimal functionality.

2. **ExampleDLL2.dll:**
   - Demonstrates more advanced features or additional capabilities.

## Screenshots

![DLL Injection Example 1](placeholder_image_1.png)

*Description of the action in the screenshot.*

![DLL Injection Example 2](placeholder_image_2.png)

*Description of the action in the screenshot.*

## License

This project is licensed under the [Apache 2.0](LICENSE).

## Disclaimer

**Use this tool responsibly. Unauthorized access to or modification of system files and processes may violate terms of service and legal agreements. The authors are not responsible for any misuse or damage caused by this software.**
