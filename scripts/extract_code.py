# -*- coding: utf-8 -*-
# extract_code.py — Extract code files into LaTeX lstlisting blocks
# Usage: python extract_code.py <code_dir> [--output <file>]

import os, sys, argparse

LANG_MAP = {
    '.py': 'Python',
    '.m': 'matlab',
    '.R': 'R',
    '.c': 'C',
    '.cpp': 'C++',
    '.java': 'Java',
}

def extract(code_dir):
    if not os.path.isdir(code_dir):
        print(f"ERROR: {code_dir} not found", file=sys.stderr)
        return ""

    files = sorted(
        [f for f in os.listdir(code_dir) if os.path.splitext(f)[1] in LANG_MAP]
    )

    if not files:
        return "% No code files found in " + code_dir

    lines = []
    for f in files:
        ext = os.path.splitext(f)[1]
        lang = LANG_MAP.get(ext, ext[1:])

        lines.append(f"\\section{{{f}}}")
        lines.append(f"\\begin{{lstlisting}}[language={lang}]")
        with open(os.path.join(code_dir, f), 'r', encoding='utf-8', errors='replace') as fh:
            for line in fh:
                lines.append(line.rstrip('\n'))
        lines.append("\\end{lstlisting}")
        lines.append("")

    return '\n'.join(lines)

def main():
    parser = argparse.ArgumentParser(description="Extract code files to LaTeX lstlisting")
    parser.add_argument("code_dir", help="Path to code/ directory")
    parser.add_argument("--output", "-o", help="Output .tex file (default: stdout)")
    args = parser.parse_args()

    result = extract(args.code_dir)

    if args.output:
        with open(args.output, 'w', encoding='utf-8') as f:
            f.write(result)
        print(f"Written to {args.output}")
    else:
        print(result)

if __name__ == '__main__':
    main()
