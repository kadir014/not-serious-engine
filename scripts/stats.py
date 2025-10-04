"""
Personal script to gather "useless" statistics about the project.
"""

import sys
import subprocess
from pathlib import Path


class FG:
    black = "\033[30m"
    darkgray = "\033[90m"
    lightgray = "\033[37m"
    white = "\033[97m"
    red = "\033[31m"
    orange = "\033[33m"
    yellow = "\033[93m"
    green = "\033[32m"
    blue = "\033[34m"
    cyan = "\033[36m"
    purple = "\033[35m"
    magenta = "\033[95m"
    lightred = "\033[91m"
    lightgreen = "\033[92m"
    lightblue = "\033[94m"
    lightcyan = "\033[96m"

class BG:
    black = "\033[40m"
    darkgray = "\033[100m"
    lightgray = "\033[47m"
    white = "\033[107m"
    red = "\033[41m"
    orange = "\033[43m"
    yellow = "\033[103m"
    green = "\033[42m"
    blue = "\033[44m"
    cyan = "\033[46m"
    purple = "\033[45m"
    magenta = "\033[105m"
    lightred = "\033[101m"
    lightgreen = "\033[102m"
    lightblue = "\033[104m"
    lightcyan = "\033[106m"

RESET = "\033[0m"


LANG_COLORS = {
    "C": FG.blue,
    "C/C++ Header": FG.blue,
    "Meson": FG.lightgreen,
    "Markdown": FG.yellow,
    "GLSL": FG.purple,
    "Python": FG.lightcyan
}


EXCLUDE_DIRS = (
    ".git",
    ".vscode",
    "build",
    "subprojects",
    "external"
)

ROOT_DIR = Path.cwd()
ROOT_DIR_NAME = str(ROOT_DIR.parts[-1])


def parse_cloc() -> tuple[str, list]:
    """ Parse cloc output and return colored chart. """

    # This assumes the script is run at root directory.
    out = subprocess.check_output(
        f"cloc . --exclude-dir={','.join(EXCLUDE_DIRS)}",
        shell=True
    )

    # Parse cloc's chart

    chart = ""
    chart_started = False
    total_values = [] # files, blank, comment, code

    for line in out.decode("utf-8").strip().split("\n"):
        line = line.strip()

        if line.startswith("-"):
            chart_started = True

        if chart_started:
            if line.startswith("-"):
                rendered = f"{FG.darkgray}{line}{RESET}"
                chart += rendered + "\n"

            elif line.startswith("Language"):
                chart += line + "\n"

            elif line.startswith("SUM"):
                total_values = line[16:].split(" ")
                total_values = [v for v in total_values if v]
                rendered = line.replace("SUM:  ", f"Total:")
                chart += rendered + "\n"

            else:
                # I got these indices from the usual output of cloc
                language = line[:16]
                files = line[16:34]
                blank = line[34:49]
                comment = line[49:64]
                code = line[64:]

                lang_color = LANG_COLORS[language.strip()]

                rendered = line.replace(language, f"{lang_color}{language}{RESET}")

                chart += rendered + "\n"

    return chart, total_values


class UselessStats:
    """
    Useless stats hell yeah.
    """

    TOTAL_LINES_OF_CODE = 0
    TOTAL_FILES = 0

    TOTAL_SIZE_OF_CODE = 0
    TOTAL_NUMBER_OF_LINES_LIMIT = 0
    MOST_INDENT = 0
    MOST_INDENT_PATH = Path()
    TOTAL_POINTER_OPERATOR_USED = 0


def indent_amount(line: str) -> int:
    """ Get the amount of indentation. """

    count = 0

    for char in line:
        if char.isspace(): count += 1
        else: break

    return count

def parse_source_file(filepath: Path, verbose: bool = False) -> None:
    """ Parse source code and adjust stats. """

    if filepath.name.startswith(".git") or filepath.name == "LICENSE":
        return

    if verbose:
        print(f"Parsing {filepath}")

    with open(filepath, "r", encoding="utf-8") as file:
        content = file.read()
        lines = content.split("\n")

        UselessStats.TOTAL_SIZE_OF_CODE += len(content.encode("utf-8"))

        for line in lines:
            if "->" in line:
                UselessStats.TOTAL_POINTER_OPERATOR_USED += line.count("->")

            indent = indent_amount(line)
            if indent > UselessStats.MOST_INDENT:
                UselessStats.MOST_INDENT = indent
                UselessStats.MOST_INDENT_PATH = filepath

            if len(line) > 80:
                UselessStats.TOTAL_NUMBER_OF_LINES_LIMIT += 1

def gather_useless_data(target_path: Path, verbose: bool = False) -> None:
    """ Recursively gather useless data. """

    for child_path in target_path.iterdir():
        if child_path.is_dir():
            excluded = False
            for part in child_path.parts:
                if str(part) in EXCLUDE_DIRS:
                    excluded = True
                    break
            if excluded:
                continue
            gather_useless_data(child_path, verbose)
        else:
            parse_source_file(child_path, verbose)


if __name__ == "__main__":
    verbose = "--verbose" in sys.argv

    chart, total_values = parse_cloc()

    # Feed these from cloc output instead
    UselessStats.TOTAL_LINES_OF_CODE = sum([int(v) for v in total_values[1:]])
    UselessStats.TOTAL_FILES = total_values[0]

    gather_useless_data(ROOT_DIR, verbose)

    total_size = float(UselessStats.TOTAL_SIZE_OF_CODE)
    unit = "bytes"
    if total_size > 1024.0:
        total_size /= 1024.0
        unit = "KB"
    if total_size > 1024.0:
        total_size /= 1024.0
        unit = "MB"

    # Print paths only upto root dir
    parts = UselessStats.MOST_INDENT_PATH.parts
    new_path = Path(*parts[parts.index(ROOT_DIR_NAME):])

    print(chart)
    print(
        f"Total lines of code: {FG.lightgreen}{UselessStats.TOTAL_LINES_OF_CODE}{RESET}\n"
        f"Total size of code: {FG.yellow}{round(total_size, 2)}{RESET} {unit} ({FG.yellow}{int(UselessStats.TOTAL_SIZE_OF_CODE)}{RESET} bytes)\n"
        f"Total number of files: {FG.lightcyan}{UselessStats.TOTAL_FILES}{RESET}\n"
        "\n"
        f"Total number of lines violating line limit (80 chars): {FG.lightred}{UselessStats.TOTAL_NUMBER_OF_LINES_LIMIT}{RESET}\n"
        f"Most indentation in one line is {FG.lightred}{UselessStats.MOST_INDENT}{RESET} spaces in {FG.orange}{new_path}{RESET}\n"
        f"Total number of {FG.darkgray}'{FG.yellow}->{FG.darkgray}'{RESET} operator usage: {FG.lightcyan}{UselessStats.TOTAL_POINTER_OPERATOR_USED}{RESET}"
    )