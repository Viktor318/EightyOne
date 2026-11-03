#!/usr/bin/env python3
"""Make Source/LanguageHu.cpp from Translations/hu.tsv.

The texts of hu.tsv are UTF-8 C string contents; in the generated file the
English ones are in Windows-1252 (as in the forms and the code) and the
Hungarian ones in Windows-1250, every byte above 127 as an octal escape, so
the file itself is plain ASCII."""
import os, sys

here = os.path.dirname(os.path.abspath(__file__))
src = os.path.join(here, 'hu.tsv')
dst = os.path.join(here, '..', 'Source', 'LanguageHu.cpp')

def c_string(text, codepage):
    out = []
    for ch in text:
        if ch == '"':
            out.append('\\"')
        elif ord(ch) < 128:
            out.append(ch)
        else:
            for b in ch.encode(codepage):
                out.append('\\%03o' % b)
    return '"' + ''.join(out) + '"'

entries, seen = [], set()
for number, line in enumerate(open(src, encoding='utf-8'), 1):
    line = line.rstrip('\n')
    if not line or line.startswith('#'): continue
    fields = line.split('\t')
    if len(fields) != 4: sys.exit('%s:%d: 4 fields needed' % (src, number))
    kind, where, english, hungarian = fields
    if english in seen: sys.exit('%s:%d: twice: %s' % (src, number, english))
    seen.add(english)
    entries.append((english, hungarian))

with open(dst, 'w', newline='\n') as f:
    f.write('// Made by Translations/make_table.py from Translations/hu.tsv;\n'
            '// change that file, not this one\n\n'
            '#include <vcl4.h>\n#pragma hdrstop\n\n#include "Language.h"\n\n'
            '#pragma package(smart_init)\n\n'
            'const LanguageEntry LanguageHungarianTable[] =\n{\n')
    for english, hungarian in entries:
        f.write('        { %s,\n          %s },\n'
                % (c_string(english, 'cp1252'), c_string(hungarian, 'cp1250')))
    f.write('        { 0, 0 }\n};\n')
print('%d texts' % len(entries))
