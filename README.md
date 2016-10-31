# chemu

Chemu is a C library for emulating CHIP-8 programs.

# Build

## Unix

For compiling chemu as a shared library use:
```shell
cd chemu
make chemu
```
This will output chemu.so in the bin directory. To compile as a static library use `make chemu-static` instead.

## Windows

Build the chemu project in [chemu.sln](VS/chemu.sln) in Visual Studio. The
library will be built to the Release or Debug folder in the VS folder.

-- OR --

Use the provided makefile, [Makefile.win](chemu/Makefile.win), by running
make.bat in a visual studio command prompt.

```shell
cd chemu
# make.bat is just a shortcut for nmake -f Makefile.win /NOLOGO
make chemu
```

# Library Modules <a name="modules"></a>

| Name         | Header file          | Function prefix | Description|
|--------------|----------------------|-------------|------------|
| decode       | chemu/decode.h       | chipdec     | Contains the OPTABLE and routines for decoding CHIP-8 instructions |
| display      | chemu/display.h      | chipdisplay | Routines for drawing sprites on a ChipDisplay |
| emulation    | chemu/emulation.h    | chipemu     | Routines for emulating a CHIP-8 ROM |
| input        | chemu/input.h        | chipinput   | Routines for modifying/accessing the ChipInput type |
| instructions | chemu/instructions.h | cif         | Contains all instruction functions |
| loop         | chemu/loop.h         | chiploop    | Routines for emulation in a separate thread |
| memory       | chemu/memory.h       | chipmem     | Routines for modifying/accessing a ChipMem struct |
| stack        | chemu/stack.h        | chipstack   | Routines for modifying a ChipStack struct |
| timer        | chemu/timer.h        | chiptimer   | Encapsulates a CHIP-8 timer, which counts down at 60Hz |

To include all modules in a source file use the chemu.h header

# Naming conventions and style

* Struct names are camel case followed by `_s`
* Similar to structs, union names are camel case followed by `_u`
* Typedef names are camel case starting with an uppercase letter.
* Macro constants and constant globals should be in uppercase with underscore spacing
* All header files must have a guard
* Header guards start with an underscore followed by the header file name in uppercase followed by `_H`
* Function names are prefixed by their associated module name (see [Library Modules](#modules)) and an underscore. Following this prefix is a name in camel case or underscore delimited.
* Each typedef *should* have its own header file, there are some exceptions to
this.
* Typedefs should not define pointer types. The only exceptions are function pointers and opaque pointers (e.g. ChipTimer).
* Inline functions must be implemented using an inline guard macro. This will
allow for inline functions to be enabled/disabled at compile time

# License

The MIT License (MIT)

Copyright (c) 2015 Brennan Ringey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
