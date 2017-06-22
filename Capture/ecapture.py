#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re
import numpy as np
from CEmergent import HsCam
import matplotlib.pyplot as plt

SUCCESS = 0
ERROR = 1


class ECapture(HsCam):
    """
    Capture class will contain methods and
    attributes to capture and manager the
    camera.
    """

    def __init__(self):
        HsCam.__init__(self)
        self.uintParam = {}
        self.boolParam = {}
        self.enumParam = {}

    def detect_camera(self):
        """
        Will return index value of the camera detected
        """
        return self._detect_camera()

    def _readConfigFiles(self):
        """
        Will read the config files for parameters
        and their default values
        """
        with open('./uint32Param.conf') as f:
            for line in f:
                fil = filter(None, re.split("[, \-!?:=\n\r]+", line))
                (key, val) = fil[0], fil[1]
                self.uintParam[key] = val

        with open('./boolParam.conf') as f:
            for line in f:
                fil = filter(None, re.split("[, \-!?:=\n\r]+", line))
                (key, val) = fil[0], fil[1]
                self.boolParam[key] = val

        with open('./enumParam.conf') as f:
            for line in f:
                fil = filter(None, re.split("[, \-!?:=\n\r]+", line))
                (key, val) = fil[0], fil[1]
                self.enumParam[key] = val

    def get_camera(self, cam_index):
        """
        Obtain a lock on the camera.
        Parameters:
            Camera Index (can be obtained from detect_camera method
        """
        result = self._get_camera(cam_index)
        if result == 0:
            print "Camera Obtained"
        elif result == 290:
            print "Camera could not be found"
        elif result == 300:
            print "Camera already acquired"

    def release_camera(self):
        """
        Release the lock from the camera. Should only be called
        at the end of the lifecycle of the capture program
        """
        result = self._release_camera()
        if result == 0:
            print "Camera Released"
        else:
            print "Camera Locked by a process"

    def read_parameter(self, param):
        """
        Read the paramter passed and check if it is valid
        parameter.
        Parametes:
            parameter
        """
        self._readConfigFiles()
        if param in self.uintParam.keys():
            return self._uint_param_range(param)
        elif param in self.boolParam.keys():
            return self._bool_param_range(param)
        elif param in self.enumParam.keys():
            return self._enum_param_range(param)

    def set_parameter(self, param, value):
        """
        Set the value to the parameter. It is recommended to check
        if the parameter is valid by calling read_parameter method
        first.
        Parameters:
            parameter name
            parameter value
        """
        ## --- TODO: Write it to the files -- ##
        if param in self.uintParam.keys():
            if self._set_uint_param(param, value) == SUCCESS:
                print "{Param} Successfully Set to {Value}".format(Param=param,
                                                                   Value=value)
            else:
                print self._set_uint_param(param, value)
        elif param in self.boolParam.keys():
            print param
            if self._set_bool_param(param, value) == SUCCESS:
                print "{Param} Successfully Set to {Value}".format(Param=param,
                                                                   Value=value)

        elif param in self.enumParam.keys():
            print param
            if self._set_enum_param(param, value) == SUCCESS:
                print "{Param} Successfully Set to {Value}".format(Param=param,
                                                                   Value=value)

    def capture_raw(self, height, width, form, compression, filename):
        """
        Capture a raw image.
        """

        # -- TODO: Return Camera buffer (in the form of array)
        # and have another method write it to a file
        ##

        self._capture_raw(height, width, form, compression, filename)

    def open_cam_stream(self):
        self._open_cam_stream()

    def close_cam_stream(self):
        self._close_cam_stream()



    def capture_stack(self, nimg, fname, height, width, form="Mono8",
                      compression="RAW"):
        """
        Capture a series of images, stack, and write to file.
    
        Parameters
        ----------
        nimg : int
            The number of images to capture for the stack.
        fname : str
            The name of the stacked image to write to disk.
        height : int
            The height of the image.
        width : int
            The width of the image.
        form : str, optional
            The format of the images to capture.
        compression : str, optional
            The type of compression to use.
        """

        # -- initialize the stack
        stack = np.zeros(height * width, dtype=float)

        # -- capture images to temporary files
        tnames = ["temporary_image_{0:05}.raw".format(i) for i in range(nimg)]

        for tname in tnames:
            self.capture_raw(height, width, form, compression, tname)
            stack[...] += np.fromfile(tname, dtype=np.uint8).astype(float)
            dum         = os.remove(tname)

        (stack / nimg).round().astype(np.uint8).tofile(fname)

        return
