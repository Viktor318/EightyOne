# tc2068timeword.py <out.svg> [scale]: the Timex Computer 2068 keyboard picture with the
# TimeWord cartridge (its words above and under the keys, and no colour strip),
# drawn again at a scale (2 if not given)
import timex

# The words above and under the keys (the faces of the analysis, in their
# order): the commands of TimeWord, and the Portuguese letters above the keys
LABELS = {
    0: ('EDIT', 'CONDENSED'), 1: ('CAPS LOCK', 'ELITE'), 2: ('INSERT ON', 'UNDERLINE'),
    3: ('INSERT LINE', 'H.QUALITY'), 4: (None, 'SUBSCRIPT'), 5: (None, 'SUPERSCRIPT'),
    6: (None, 'BOLD'), 7: (None, 'D.STRIKE'), 8: ('GRAPHICS', 'D.WIDTH'), 9: ('DELETE', 'ITALIC'),
    10: ('TAB', 'QUIT'), 12: ('CURSOR HOME', 'EXIT'), 13: (None, 'REPLACE'), 14: (None, 'DEF TAB'),
    16: ('ú', None), 17: ('í', None), 18: ('ó', None), 19: (None, 'PRINT FILE'),
    20: ('STOP/á', 'ADVANCE'), 21: ('REVISE', 'SAVE'), 22: ('ê', 'DELET FILE'), 23: ('Ff', 'FIND'),
    24: ('õ', None), 25: ('à', 'HELP'), 26: ('â', None), 27: (None, 'DELET LINE'),
    28: ('ô', 'LIST DRIVE'),
    33: ('ç', 'COPY'), 35: ('ã', 'REFORM'), 36: ('ñ', None), 37: (None, 'MOVE'),
}

timex.draw('tc2068timeword', (0, 0, 0), (210, 210, 214), labels=LABELS, names=[],
           bottom=((0, 198, 62, 208, 'TIMEWORD', 'heros', 7, True), (64, 198, 92, 208, 'V 1.2', 'heros', 7, True),
                   (0, 209, 90, 217, 'PROCESSADOR DE TEXTO', 'cnb', 5)), arrows=1, room={12: 44})
