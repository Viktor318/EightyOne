# ts2068.py <out.svg> [scale]: the Timex Sinclair 2068 keyboard picture (the TC2048's in other
# colours, with its own words under five keys), drawn again at a scale (2 if not given)
import timex

timex.draw('ts2068', (0, 0, 0), (210, 210, 214),
           under={19: 'RESET', 20: 'FREE', 21: 'STICK', 23: 'ON ERR', 24: 'SOUND'})
