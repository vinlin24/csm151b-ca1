# EC ENGR M116C Computer Assignment 1: Report

* Name: Vincent Lin
* UID: 405572351
* Section: 1H


## Datapath Design

<!-- TODO: Datapath with control signals. -->


## Control Signal Reference

<!-- TODO: Table that shows the values for each control signal for all
instructions. -->


## Answers to Questions

> **What is the total number of cycles for running "all" trace (ZERO instruction
> included)?**

Since we're assuming a single-cycle processor design, the number of cycles to
run the trace is simply equal to the number of assembly instructions provided in
`23all.s`, which is 14. Adding one for the ZERO instruction (the program
termination condition) gives us **15 total cycles**.

> **How many r-type instructions does this program ("all") have?**

The R-type instructions that we implemented are `ADD`, `SUB`, `XOR`, and `SRA`.
`23all.s` includes 1 `SUB` instruction and 1 `ADD` instruction, so the "all"
program has a total of **2 R-type instructions**.

> **What is the IPC of this processor (for "all" trace)?**

Since we're assuming a single-cycle processor design, the instructions per cycle
(IPC) is equal to **1** by definition.
