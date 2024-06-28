#!/usr/bin/env python3

import re

from path import Path
from rich import print


def parse_predicates_md(file_path: Path) -> set[str]:
    fp_predicates: set[str] = set()
    current_predicate: str | None = None

    with open(file_path) as file:
        for line in file:
            if line.startswith(";;"):
                continue
            match = re.search(r'\(define_predicate\s+"([^"]+)"', line)
            if match:
                current_predicate = match.group(1)
            elif current_predicate:
                if "ANY_FP_REGNO_P" in line or "STACK_REGNO_P" in line:
                    fp_predicates.add(current_predicate)
                if line.strip() == ")":
                    current_predicate = None

    return fp_predicates


def parse_i386_md_for_predicates(file_path: Path, fp_predicates: set[str]) -> dict[str, set[str]]:
    pattern_feature_map: dict[str, set[str]] = {}
    current_feature = None
    current_pattern = None

    with open(file_path) as file:
        for line in file:
            if line.startswith("(define_insn"):
                current_feature = None
                current_pattern = None
            elif "TARGET_" in line:
                current_feature = line.strip().split(" ")[-1]
            elif current_feature:
                if any(pred in line for pred in fp_predicates):
                    current_pattern = line.strip()
                    if current_feature not in pattern_feature_map:
                        pattern_feature_map[current_feature] = set()
                    pattern_feature_map[current_feature].add(current_pattern)

    return pattern_feature_map


def parse_i386_opt(file_path: Path) -> dict[str, str]:
    feature_flag_map: dict[str, str] = {}

    with open(file_path) as file:
        for line in file:
            match = re.search(r"^(m[a-zA-Z0-9_-]+)", line)
            if match:
                feature = match.group(1)
                feature_flag_map[feature] = line.strip()

    return feature_flag_map


def map_features_to_flags(
    pattern_feature_map: dict[str, set[str]], i386_opt_path: Path
) -> list[str]:
    feature_flag_map = parse_i386_opt(i386_opt_path)
    flags: list[str] = []

    for feature in pattern_feature_map:
        for flag in feature_flag_map:
            if feature.lower() in flag.lower():
                flags.append(f"-mno-{flag}")

    return flags


def main() -> None:
    # Paths to the relevant files
    predicates_md_path = Path("gcc/config/i386/predicates.md")
    i386_md_path = Path("gcc/config/i386/i386.md")
    i386_opt_path = Path("gcc/config/i386/i386.opt")

    fp_predicates = parse_predicates_md(predicates_md_path)
    print(f"fp_predicates: {fp_predicates}")
    pattern_feature_map = parse_i386_md_for_predicates(i386_md_path, fp_predicates)
    print(f"pattern_feature_map: {pattern_feature_map}")
    mno_flags = map_features_to_flags(pattern_feature_map, i386_opt_path)
    print(f"mno_flags: {mno_flags}")

    # Output the flags
    for flag in mno_flags:
        print(flag)


if __name__ == "__main__":
    main()
