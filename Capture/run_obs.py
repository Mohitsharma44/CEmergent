#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import time
import keyboard
import schedule
import numpy as np
from ecapture import ECapture
import matplotlib.pyplot as plt


def do_capture6(ec):
    """
    Wrapper for the capture_stack method of ec to facilitate scheduler.

    Parameters
    ----------
    ec : ECapture
       An ECapture instance.
    """

    # -- unpack parameters
    nimg = params[0]
    base = params[1]
    hgt  = params[2]
    wid  = params[3]

    # -- capture six stacks (waiting 10 seconds between captures)
    for ii in range(6):
        t0 = time.time()
        ec.capture_stack(nimg, base.format(cnt[0]), hgt, wid)

        # -- update counter
        cnt[0] += 1

        # -- capture next stack 10s after the beginning of the current stack
        time.sleep(10. - (time.time() - t0))

    
    return


def pan_to_position(pan, tilt):
    """
    Move the pan/tilt to a specified position.

    Patameters
    ----------
    pan : int
        The pan position.
    tilt : int
        The tilt position.
    """

    # -- get pan/tilt coords, move pt system, and release
    #    NB, good value is 95 -225
    kc = keyboard.KeyboardController(pt_ip="192.168.1.50", pt_port=4000)
    kc.pan(pan)
    kc.tilt(tilt)
    kc.cleanup()

    return


if __name__ == "__main__":

    # -- utilities
    cnt    = [3240]
    pt     = (95, -255)
    nimg   = 20
    tint   = 33188 # ms
    params = [nimg, "stack_bb_{0:07}.raw", 0, 0, tint]


    # -- set pan/tilt params and move to position
    pan_to_position(pt[0], pt[1])


    # -- set up the image capture
    ec   = ECapture()
    ec.get_camera(ec.detect_camera())


    # -- read the configuration files
    ec._readConfigFiles()


    # -- get parameters necessary to start camera stream
    wid       = ec.read_parameter('Width')[0]
    hgt       = ec.read_parameter('Height')[0]
    params[3] = wid
    params[2] = hgt


    # -- open the camera stream, disbale HDR and white balance, set exposure
    ec.open_cam_stream()
    ec.set_parameter('HDREnable', False)
    ec.set_parameter('WB_Enable', False)
    ec.set_parameter('Exposure', params[4])


    # -- start image acquisition
    try:
        # -- set up the scheduler
        sec   = np.arange(21 * 3600, (21 + 9) * 3600, 60)
        times = ["{0:02}:{1:02}".format(i, j) for i, j in
                 zip(sec // 3600 % 24, sec % 3600 // 60)]
        dum   = [schedule.every().day.at(i).do(do_capture6, ec) for i in
                 times]

        # -- run scheduler
        while True:
            schedule.run_pending()
            time.sleep(0.1)

        # -- release camera
        ec.close_cam_stream()
        ec.release_camera()
    except Exception as exc:
        print("CAPTURE {0} FAILED!!!".format(cnt[0]))
        print(exc)

        # -- release camera
        ec.close_cam_stream()
        ec.release_camera()
