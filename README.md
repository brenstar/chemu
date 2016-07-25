# chemu

Chemu is a C library for emulating CHIP-8 programs.

# Library Modules <a name="modules"></a>

| Name         | Header file          | Function prefix | Description|
|--------------|----------------------|-------------|------------|
| decode       | chemu/decode.h       | chipdec     | Contains the OPTABLE and routines for decoding CHIP-8 instructions |
| display      | chemu/display.h      | chipdisplay | Routines for drawing sprites on a ChipDisplay |
| emulation    | chemu/emulation.h    | chipemu     | Contains the main loop |
| input        | chemu/input.h        | chipinput   | Routines for modifying/accessing the ChipInput type |
| instructions | chemu/instructions.h | cif         | Contains all instruction functions |
| memory       | chemu/memory.h       | chipmem     | Routines for modifying/accessing a ChipMem struct |
| stack        | chemu/stack.h        | chipstack   | Routines for modifying a ChipStack struct |
| timer        | chemu/timer.h        | chiptimer   | Encapsulates a CHIP-8 timer, which counts down at 60Hz |

# Naming conventions and Style

* Struct names are camel case followed by `_s`
* Similar to structs, union names are camel case followed by `_u`
* Typedef names are camel case starting with an uppercase letter.
* Macro constants and constant globals should be in uppercase with underscore spacing
* Header guards start with an underscore followed by the header file name in uppercased followed by `_H`
* Function names are prefixed by their associated module name (see [Library Modules](#modules)) and an underscore. Following this prefix is a name in camel case starting with a lowercase letter.
* Each typedef should have its own header file
* Typedefs should not define pointer types. The only exceptions are function pointers and opaque pointers (e.g. ChipTimer).

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
