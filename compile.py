#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Compile a RISC-V .s file into the instMem format."""

from argparse import ArgumentParser
import io
from pathlib import Path
import sys
from typing import Literal

__author__ = "Vincent Lin"

parser = ArgumentParser(
    prog=Path(sys.argv[0]).name,
    description=__doc__,
)
parser.add_argument(
    "source_path",
    metavar="SOURCE",
    type=Path,
    help=".s file with just the assembly code, no annotations",
)
parser.add_argument(
    "-o", "--output",
    metavar="DEST",
    dest="dest_path",
    type=Path,
    help="instMem file to save the output to",
)

R_Type = Literal["add", "sub", "xor", "sra"]
I_Type = Literal["addi", "andi"]


def compile_r_type(rd: str, rs1: str, rs2: str, command: R_Type) -> str:
    opcode = "0110011"
    if command in {"add", "xor"}:
        funct7 = "0000000"
    else:
        funct7 = "0100000"
    if command in {"add", "sub"}:
        funct3 = "000"
    elif command == "xor":
        funct3 = "100"
    else:  # command == "sra"
        funct3 = "101"
    return funct7 + rs2 + rs1 + funct3 + rd + opcode


def compile_i_type(rd: str, rs1: str, immediate: str, command: I_Type) -> str:
    opcode = "0010011"
    if command == "addi":
        funct3 = "000"
    else:  # command == "andi"
        funct3 = "111"
    return immediate + rs1 + funct3 + rd + opcode


def compile_lw(rd: str, rs1: str, immediate: str) -> str:
    opcode = "0000011"
    funct3 = "010"
    return immediate + rs1 + funct3 + rd + opcode


def compile_sw(rs1: str, rs2: str, immediate: str) -> str:
    opcode = "0100011"
    funct3 = "010"
    return immediate[:7] + rs2 + rs1 + funct3 + immediate[7:] + opcode


def compile_blt(rs1: str, rs2: str, immediate: str) -> str:
    opcode = "1100011"
    funct3 = "100"
    return (immediate[0] + immediate[2:8]
            + rs2 + rs1 + funct3
            + immediate[8:12] + immediate[1]
            + opcode)


def compile_jalr(rd: str, rs1: str, immediate: str) -> str:
    opcode = "1100111"
    funct3 = "000"
    return immediate + rs1 + funct3 + rd + opcode


def register_to_binary(name: str) -> str:
    register_num = int(name.removeprefix("x"))
    binary_string = bin(register_num).removeprefix("0b")
    return binary_string.zfill(5)


def immediate_to_binary(token: str, length: int) -> str:
    def twos_complement(num: int) -> str:
        """Thanks ChatGPT."""
        if num >= 0:
            binary = bin(num)[2:].zfill(length)
        else:
            binary = bin(2**length + num)[2:]
        return binary

    magnitude = token.removeprefix("-")
    if magnitude.startswith("0x"):
        value = int(magnitude.removeprefix("0x"), 16)
    else:
        value = int(magnitude)
    if token.startswith("-"):
        value = -value
    return twos_complement(value)


def binary_to_little_endian(instruction: str) -> tuple[int, int, int, int]:
    byte3 = instruction[0:8]
    byte2 = instruction[8:16]
    byte1 = instruction[16:24]
    byte0 = instruction[24:32]
    return (
        int(byte0, 2),
        int(byte1, 2),
        int(byte2, 2),
        int(byte3, 2),
    )


def tokenize_line(line: str) -> list[str]:
    """
    I can't just do line.split() because for some reason the handout
    trace files have inconsistent spacing between operands, with some
    operands stuck to the comma on both sides.
    """
    tokens = list[str]()
    token = ""
    for char in line:
        if char in ", ":
            if token != "":
                tokens.append(token)
            token = ""
        else:
            token += char
    if token != "":
        tokens.append(token)
    return tokens


def compile_assembly(source_path: Path) -> str:
    assembly_source = source_path.read_text(encoding="utf-8").strip()
    assembly_lines = assembly_source.splitlines()

    output = io.StringIO()
    for line in assembly_lines:
        tokens = tokenize_line(line)
        command = tokens[0]
        operand1 = tokens[1].rstrip(",")
        operand2 = tokens[2].rstrip(",")
        operand3 = tokens[3]

        # erm ackshually this code doesn't look very DRY- stfu this was
        # hacked together.
        instr: str
        match command:
            case "add" | "sub" | "xor" | "sra":
                rd_bin = register_to_binary(operand1)
                rs1_bin = register_to_binary(operand2)
                rs2_bin = register_to_binary(operand3)
                instr = compile_r_type(rd_bin, rs1_bin, rs2_bin, command)
            case "addi" | "andi" | "jalr":
                rd_bin = register_to_binary(operand1)
                rs1_bin = register_to_binary(operand2)
                immediate_bin = immediate_to_binary(operand3, 12)
                if command == "jalr":
                    instr = compile_jalr(rd_bin, rs1_bin, immediate_bin)
                else:
                    instr = compile_i_type(rd_bin, rs1_bin, immediate_bin, command)
            case "lw":
                rd_bin = register_to_binary(operand1)
                immediate_bin = immediate_to_binary(operand2, 12)
                rs1_bin = register_to_binary(operand3)
                instr = compile_lw(rd_bin, rs1_bin, immediate_bin)
            case "sw":
                rs2_bin = register_to_binary(operand1)
                immediate_bin = immediate_to_binary(operand2, 12)
                rs1_bin = register_to_binary(operand3)
                instr = compile_sw(rs1_bin, rs2_bin, immediate_bin)
            case "blt":
                rs1_bin = register_to_binary(operand1)
                rs2_bin = register_to_binary(operand2)
                immediate_bin = immediate_to_binary(operand3, 13)
                instr = compile_blt(rs1_bin, rs2_bin, immediate_bin)
            case _:
                raise ValueError(f"unrecognized command {command}")

        instruction_bytes = binary_to_little_endian(instr)
        for byte in instruction_bytes:
            output.write(f"{byte}\n")

    return output.getvalue()


def main() -> None:
    namespace = parser.parse_args()
    source_path: Path = namespace.source_path
    dest_path: Path | None = namespace.dest_path

    output = compile_assembly(source_path)
    if dest_path is None:
        print(output)
    else:
        dest_path.write_text(output, encoding="utf-8", newline="\n")


if __name__ == "__main__":
    main()
