#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import keyboard
import numpy as np
from ecapture import ECapture
import matplotlib.pyplot as plt


# -- get pan/tilt coords, move pt system, and release
#    NB, good value is 95 -225
pt = (int(sys.argv[1]), int(sys.argv[2]))
kc = keyboard.KeyboardController(pt_ip="192.168.1.50", pt_port=4000)
kc.pan(pt[0])
kc.tilt(pt[1])
kc.cleanup()

# -- set up the image capture
ec   = ECapture()
cind = ec.detect_camera()
ec.get_camera(cind)

# -- read the configuration files
ec._readConfigFiles()

# -- get parameters necessary to start camera stream
wid = ec.read_parameter('Width')[0]
hgt = ec.read_parameter('Height')[0]
# GGD: why are the below necessary?
ec.set_parameter('Width', wid)
ec.set_parameter('Height', hgt)

# -- open the camera stream
ec.open_cam_stream()

# -- disable the high dynamic range
ec.set_parameter('HDREnable', False)

# -- set the exposure
# ec.set_parameter('Exposure', 33188)
ec.set_parameter('Exposure', 30)

# -- turn off white balance
ec.set_parameter('WB_Enable', False)


try:
    # -- get a stack
    ec.capture_stack(10, "temp.raw", hgt, wid)

    # -- read the image
    print("reading file...")
    img = np.fromfile("temp.raw", np.uint8).reshape(hgt, wid)
        
    # -- visualize
    print("plotting...")
    xs = 8.0
    fig, ax = plt.subplots(figsize=(xs, xs * float(hgt)/float(wid)))
    fig.subplots_adjust(0, 0, 1, 1)
    ax.axis("off")
    ax.imshow(img[::2, ::2]**0.5, "gist_gray")
    fig.canvas.draw()
    plt.show()
    
    # -- release camera
    ec.close_cam_stream()
    ec.release_camera()
except Exception as exc:
    print("CAPTURE FAILED!!!")
    print(exc)

    # -- release camera
    ec.close_cam_stream()
    ec.release_camera()
