<!-- PROJECT TITLE -->
<br />
<h1 align="center">Notation Converter CLI</h1>

  <p align="center">
    A command-line utility in C to convert mathematical expressions between infix, prefix, and postfix notations using expression trees.
    <br />
    <a href="https://github.com/github_username/repo_name"><strong>Explore files >></strong></a>
    <br />
    <br />
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#program-overview">Program Overview</a>
      <ul>
        <li><a href="#introduction">Introduction</a></li>
        <li><a href="#key-features">Key Features</a></li>
      </ul>
    </li>
    <li>
      <a href="#setup-guide">Setup Guide</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#program-execution">Program Execution</a></li>
    <ul>
        <li><a href="#compilation">Compilation</a></li>
        <li><a href="#examples">Examples</a></li>
      </ul>
    <li><a href="#authors">Authors</a></li>
    <li><a href="#references">References</a></li>
  </ol>
</details>

<!-- PROGRAM OVERVIEW -->
## Program Overview

### Introduction
The **Notation Converter CLI** is a command-line utility written in C that allows users to convert mathematical expressions between ***infix***, ***prefix***, and ***postfix*** notations. It utilizes expression trees and their corresponding traversals (_in-order_, _pre-order_, and _post-order_) to parse the expression and systematically generate the equivalent forms in the desired notation. This program is useful for learning how expressions are represented and evaluated by computers, especially in the fields of data structures, compilers, and programming languages.

### Key Features
* **Command-Line Interface (CLI)**
  + The program uses a command-line interface, allowing users to input expressions and specify their desired operations through command-line arguments. This means users run the program by typing commands with options directly in the terminal, without any interactive menus.
* **Input Checking and Validation**
  + The program checks if the input expression is valid before processing it. It ensures that parentheses are balanced, the number of operands and operators match, and the expression follows the correct format for the selected notation.
* **Expression Tree Construction**
  + The program constructs an internal expression tree to represent the logical structure of the arithmetic expression. This tree enables precise transformation between different notations.
* **Expression Conversion Between Notations**
  + The program supports conversion between infix, prefix, and postfix expressions. Based on user input, it determines the input and output notation and performs accurate conversions.
  + Once the expression tree is built, the program can convert the expression into other notations by traversing the tree in different ways:
    - In-order traversal produces the infix form of the expression.
    - Pre-order traversal produces the prefix form of the expression.
    - Post-order traversal produces the postfix form of the expression.
* **Modular and Reusable Design**
  + The code is organized into distinct modules for parsing, validation, tree construction, conversion, and other helper functions. This modularity makes the program easier to understand, debug, and extend with additional features.
* **Error Handling**
  + The program includes error handling to detect and respond to common issues, such as missing arguments, invalid notations, or incorrect expression formats. Clear and helpful error messages are displayed to guide users in correcting their input.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- SETUP GUIDE -->
## Setup Guide

To run the Notation Converter CLI, ensure your system meets the following prerequisites and has the necessary tools installed.

### Prerequisites
* **Operating System**:  
  + The program has been tested on and is compatible with the following operating systems:  
    - Windows 
    - macOS  
    - Linux
* **Terminal/Command Line Interface**:
  + Required for compiling and executing the program. 
* **C Compiler**:
  + This project is written in the C programming language and must be compiled before use. The most common and recommended compiler is GCC (GNU Compiler Collection).
  + To check if GCC is installed, run the following command in your terminal or command prompt:
    ```sh
    gcc --version
    ```
  + If GCC is not installed, follow the instructions below based on your operating system:


**Operating System:**

1. **Windows** (via MinGW):
      + Download **MinGW - Minimalist GNU for Windows** from [https://osdn.net/projects/mingw/releases/.](https://sourceforge.net/projects/mingw/)
      + Run the installer and select the package: mingw32-gcc-g++ under **Basic Setup**.
      + Click Installation and apply changes in the menu to complete installation.
      + After installation, verify GCC:
        ```sh
        gcc --version
        ```
2. **macOS** (via Homebrew):
      + Install **Homebrew** (if not already installed), then open terminal and run:
        ```sh
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        ```
      + Once Homebrew is installed, run:
        ```sh
        brew install gcc
        ```
      + After installation, verify GCC:
        ```sh
        gcc --version
        ```
3. **Linux** (via Ubuntu):
      + Update the package list by opening the terminal and running
        ```sh
        sudo apt update
        ```
      + Install essential development tools including GCC and G++:
        ```sh
        sudo apt install build-essential
        ```
        This installs:
           - `gcc` (C compiler)  
           - `g++` (C++ compiler)  
           - `make` and other development tools  
      + After installation, verify GCC:
        ```sh
        gcc --version
        ```

### Installation
Once your system meets the prerequisites, follow these steps to install and set up the Notation Converter CLI.
   
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- PROGRAM EXECUTION -->
## Program Execution

### Compilation

To compile the program, ensure you have a C compiler installed. The most commonly used compiler is `gcc` (GNU Compiler Collection), which is available on all major platforms.
* For **Windows**, use the following command:
  ```sh
     gcc notation-converter.c -o notation-converter.exe
  ```
    > * `gcc`: The GNU Compiler Collection, commonly used for compiling C programs.
    > * `notation-converter.c`: The main C source file of the program.
    > * `-o notation-converter.exe`: Specifies the name of the output executable file.
* For **Linux** or **macOS**, use the same command, adjusting the output file name if desired:
  ```sh
     gcc notation-converter.c -o notation-converter
  ```
    > * Unlike Windows, the `.exe` extension is not required on Unix-based systems.

### Usage

After compilation, the program is executed from the command line using a specific format.
1. To convert an expression:
   ```sh
      notation-converter --from <input_format> --to <output_format> "<expression>"
   ```
   > * `notation-converter`: The command used to run the program in the terminal.
   >   + The exact format depends on your operating system: use `notation-converter.exe` on Windows, and `./notation-converter` on Linux or macOS.
   > * `--from <input_format>`: Specifies the format of the input expression.
   > * `--to <output_format>`: Specifies the desired output format.
   > * `"<expression>"`: The expression to be converted. It must be enclosed in double quotes.
2. To display the `help` option with a brief usage summary:
   * Either:
     ```sh
        notation-converter -h
     ```
   * Or:
     ```sh
        notation-converter --help
     ```
3. To display the `guide` option for more detailed information about the program:
```sh
   notation-converter --guide
```

### Examples

Here are some sample usages of the program:
1. Convert an prefix expression to infix:
   ```sh
      notation-converter --from prefix --to infix "* + 1 2 3"
   ```
   Output:
   ```css
      ( ( 1 + 2 ) * 3 )
   ```
2. Convert an infix expression to postfix:
   ```sh
      notation-converter --from infix --to postfix "( 1 + 2 ) * 3"
   ```
   Output:
   ```css
      1 2 + 3 *
   ```
3. Convert a postfix expression to prefix:
   ```sh
      notation-converter --from postfix --to prefix "1 2 3 * +"
   ```
   Output:
   ```css
      + 1 * 2 3
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- AUTHORS -->
## Authors

This program was developed by the following students of BS Computer Science at Bicol Univeristy - College of Science:
- A Z Rain L. Espinas
- Jaycee D. Cadag
- John Melrick M. Loviña

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- REFERENCES -->
## References

Rosen, Kenneth H. *Discrete Mathematics and Its Applications* (8th ed.). McGraw-Hill Education, 2019. ISBN: 9781259676512.

<p align="right">(<a href="#readme-top">back to top</a>)</p>
