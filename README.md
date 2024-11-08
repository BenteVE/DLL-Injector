# DLL Injector for Windows (x86/x64)

## Overview

This repository contains a simple DLL (Dynamic Link Library) injector that uses the Windows API `LoadLibrary` function.
The injector allows you to inject custom DLLs into running processes, enabling you to modify their behavior dynamically.

This implementation can be build for both x86 and x64 architectures in Visual Studio.
To test the injector, the repository also includes a project to build a simple DLL that opens a console in the target application when injected.

## Usage

1. **Build the Injector:**

   Note that the injector, injected DLL and target program should all have the same architecture (x86 or x64)!

   - Open the `DLLInjector.sln` solution file in Visual Studio.
   - Build the solution for the desired architecture (x86 or x64).

2. **Build or locate the DLL you want to inject**

3. **Prepare Target Process:**
   - Run the target application you want to inject the DLL into.
   - Identify the target process using the PID, the application name, or the window title

4. **Run the Injector from the command line:**

   This simple injector is built for educational purposes and makes no attempt to hide it's activity.
   If Windows Defender is active, it will likely detect this behaviour, interpret it as suspicious, and remove the injector.
   You can disable Windows Defender temporarily at your own risk.

   ```cmd
   .\DLLInjector_x86.exe .\DLL-Open-Console_x86.dll --process-name Notepad++.exe
   ```

5. **Verify that the DLL is successfully injected**

   The console should show the following output:

   ![DLL Injection Console](doc/injector-console.png)

   With the provided DLL, a console window should now be opened for the target application.

   ![DLL Open Console](doc/injector-notepad.png)

6. **Eject the DLL (optional):**

   Press 'e' to eject, the opened console should then close.

   ![DLL Eject](doc/injector-console-eject.png)

## Injection Process

The injector uses the following steps to inject the DLL in the target process:

1. Determine the process ID of the target. The Windows API contains several functions that can accomplish this (`CreateToolhelp32Snapshot` or `GetWindowThreadProcessId`). Alternatively, you can also use the Windows Task Manager.

2. Use the `OpenProcess` function to get the `Handle` of the target process.
   For the following steps, it is important to use the `PROCESS_ALL_ACCESS`-flag.

   ![DLL Injection OpenProcess](doc/injector-OpenProcess.png)

3. Use the `VirtualAllocEx` function to reserve a space in the memory of the target process.
   We need both `read` and `write` access to the memory.

   ![DLL Injection VirtualAllocEx](doc/injector-VirtualAllocEx.png)

4. Use the `WriteProcessMemory` function to store the path to the chosen DLL in the memory of the target process that we reserved in the previous step.

   ![DLL Injection WriteProcessMemory](doc/injector-WriteProcessMemory.png)

5. Use the `CreateRemoteThread` function to create a new thread in the target process.
   This thread will then call the `LoadLibrary` function to load the DLL in the target process.

   ![DLL Injection CreateRemoteThread](doc/injector-CreateRemoteThread.png)

   When the injectory is finished, the chosen DLL will be loaded in the address space of the target process:

   ![DLL Injection Result](doc/injector-result.png)
