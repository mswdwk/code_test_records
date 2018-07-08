#!/usr/bin/env python
 
import matplotlib
matplotlib.use('Agg')
 
 
import matplotlib.pyplot as plot
from pylab import arange, sin, pi
 
 
fig = plot.figure()
ax = fig.add_subplot(111)
t = arange(0.0, 1.0, 0.01)
s = sin(2 * pi * t)
ax.plot(t, s)
ax.autoscale_view()
ax.grid(True)
plot.savefig('plot.png')
