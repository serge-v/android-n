#!/usr/bin/python

import os, sys

# def generate(start, stop):
    # for i in range(ord(start[0]), ord(stop[0])+1):
        # print '%s: %02x' % (chr(i), i)
        # cmd = 'convert -resize 32x48\! -font nimbus-mono-regular -pointsize 300 label:%s char_%04x.xbm' % (chr(i), i)
        # os.system(cmd)
        # cmd = 'convert -resize 32x48\! -font nimbus-mono-regular -pointsize 300 label:%s char_%04x.png' % (chr(i), i)
        # os.system(cmd)

# generate('A', 'Z')
# generate('0', '9')

for i in range(ord(u'А'), ord(u'Я')+1):
    cmd = u'convert -resize 32x48\! -font nimbus-mono-regular -pointsize 300 label:%s char_%04x.xbm' % (unichr(i), i)
    print cmd
    cmd = u'convert -resize 32x48\! -font nimbus-mono-regular -pointsize 300 label:%s char_%04x.png' % (unichr(i), i)
    print cmd
