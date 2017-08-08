#!/usr/bin/python
import svgwrite
import struct
import time
import sys

infile_path = "/dev/input/event" + (sys.argv[1] if len(sys.argv) > 1 else "19")

#long int, long int, unsigned short, unsigned short, unsigned int
FORMAT = 'llHHI'
EVENT_SIZE = struct.calcsize(FORMAT)

#open file in binary mode
in_file = open(infile_path, "rb")
svg_size_width = 1750
svg_size_height = 1950

dwg = svgwrite.Drawing('test.svg', (svg_size_width, svg_size_height))
dwg.add(dwg.rect(insert=(0, 0), size=('100%', '100%'), rx=None, ry=None, fill='rgb(255,255,255)'))

event = in_file.read(EVENT_SIZE)

x=None
y=None
oldX=None
oldY=None
eventcounter=0

#types:
# 3: move
# 1: pen up

while event:
    (tv_sec, tv_usec, type, code, value) = struct.unpack(FORMAT, event)

    if type != 0 or code != 0 or value != 0:
        if type == 1:
                x=None
                y=None
                oldX=None
                oldY = None
    	if type == 3 and code == 00:
    			#print("x: %u") % value
                oldX=x
                x=value
                eventcounter+=1
    	if type == 3 and code == 01:
    			#print("y: %u") % value
                oldY=y
                y=value
                eventcounter+=1
        if x!=None and y!=None and oldX!=None and oldY!=None:
                #print("x,y: %u,%u") %(x,y)
                #print eventcounter
                if x>1500 and y>1880:
                        dwg.save()
                        print
                        print 'Created SVG. Exiting'
                        break
                else:
                    if eventcounter%2==0:
                        eventcounter=0
                        #dwg.add(dwg.line((oldX, oldY), (x, y), stroke=svgwrite.rgb(0, 0, 0, '%')))
                        #dwg.add(dwg.line((oldX, oldY), (x, y), stroke='red', stroke_width=3))
                        #dwg.add(dwg.line((oldX, oldY), (x, y), stroke='red', stroke_width=3))
                        p = dwg.path(d="M {0},{1} m {2},{3} z".format(oldX, oldY, x, y), stroke="red", stroke_width=3)
                        dwg.add(p)
    
                        print 'added path from %u,%u to %u,%u' %(oldX, oldY, x, y)

        #print("Event type %u, code %u, value %u at %d.%d" % \
        #    (type, code, value, tv_sec, tv_usec))
    else:
        # Events with code, type and value == 0 are "separator" events
        #print("===========================================")
        pass
    event = in_file.read(EVENT_SIZE)

in_file.close()
