# DLL Injector for Windows (x86/x64)

## Overview

This repository contains a simple DLL (Dynamic Link Library) injector designed for Windows programs, supporting both x86 and x64 architectures. The injector allows you to inject custom DLLs into running processes, enabling you to modify their behavior dynamically.

![DLL Injection Example 1](placeholder_1.gif)

## Features

- **Cross-Architecture Support:** Able to compile and work with both x86 and x64 Windows programs.
- **Example DLL:** Includes an example DLL to test the injector that opens a console in the target process.
- **Visual Studio Project:** The injector is developed using Microsoft Visual Studio for easy compilation and modification.

## Usage

1. **Build the Injector:**
   - Open the `DLLInjector.sln` solution file in Visual Studio.
   - Build the solution for the desired architecture (x86 or x64).
   - Note: the injector and the target program should have the same architecture!

2. **Build the DLL:**
   - Open the `DLL-Open-Console.sln` solution file in Visual Studio.
   - Build the solution for the desired architecture (x86 or x64).
   - Note: the DLL and the target program should have the same architecture!

3. **Prepare Target Process:**
   - Run the target application you want to inject the DLL into.
   - Identify the target process using the PID, the application name, or the window title

4. **Run the Injector from the command line:**

   ```cmd
   .\DLLInjector.exe .\DLL-Open-Console.dll process_name Notepad.exe
   ```

5. **Verify that the DLL is successfully injected:**

![DLL Injection Example 1](placeholder_image_1.png)

6. **Eject the DLL (optional):**
   - Press 'e' to eject

## Injection Process

![DLL Injection Example 1](placeholder_image_2.png)

## DLL Open Console

When this DLL is injected in a target process, it opens a console in the target application.

![DLL Open Console](placeholder_image_3.png)

## License

This project is licensed under the [MIT](LICENSE).

## Disclaimer

**Use this tool responsibly. Unauthorized access to or modification of system files and processes may violate terms of service and legal agreements. The authors are not responsible for any misuse or damage caused by this software.**
