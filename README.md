# RISC-V CPU Simulator

<!-- Clear READMEs save lives. -->


## Description

Computer Assignment 1 (CA1) for UCLA F23 **COM SCI M151B/EC ENGR M116C: Computer
Systems Architecture**.

The goal of this computer assignment is to create a simulator for a (simple)
RISC-V CPU. The rest of the details can be found in [the spec](CA1.pdf).


## Building

To build the main executable locally, run the Makefile rule:

```sh
make
```

This will create a binary named `cpusim` in the current directory.

Note that the source files are deliberately at the root of the project directory
because that is the directory structure the Gradescope auto-grader expects.


## Usage

From [the spec](CA1.pdf):

The entry point of your project is "cpusim.cpp". The program should run like
this:

```sh
./cpusim <inputfile.txt>
```

...and print the value of `a0` and `a1` in the terminal:

```console
(a0,a1)
```

(no space). For example, if `a0=10`, and `a1=-8`, then you should print
`(10,-8)`.

> You should use exactly this format, otherwise, our automated tests cannot
evaluate your code.

Trace files are provided under the [traces/](traces/) subdirectory.


## Cleanup

To clean the project directory of this binary and intermediate object files, run
this Makefile rule:

```sh
make clean
```


## Submission

* The code source files for submission are the `*.cpp` and `*.h` files in the
  project root.
* The project report is [report.pdf](report.pdf) (built from the
  [report.md](report.md) Markdown source).
