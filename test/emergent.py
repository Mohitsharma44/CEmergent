import re
#import CEmergent as ce
from CEmergent import HsCam

#cam = ce.HsCam()

SUCCESS = 0
ERROR = 1


class ECapture(HsCam):
    def __init__(self):
        #self.cam = HsCam()
        HsCam.__init__(self)
        self.uintParam = {}
        self.boolParam = {}
        self.enumParam = {}

    def detect_camera(self):
        return self._detect_camera()
        
    def _readConfigFiles(self):
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
        result = self._get_camera(cam_index)
        if result == 0:
            print "Camera Obtained"
        elif result == 290:
            print "Camera could not be found"
        elif result == 300:
            print "Camera already acquired"
            
    def release_camera(self):
        result = self._release_camera()
        if result == 0:
            print "Camera Released"
        else:
            print "Camera Locked by a process"
            
    def read_parameter(self, param):
        self._readConfigFiles()
        if param in self.uintParam.keys():
            print self._uint_param_range(param)
        elif param in self.boolParam.keys():
            print self._bool_param_range(param)
        elif param in self.enumParam.keys():
            print self._enum_param_range(param)
                    
    def set_parameter(self, param, value):
        ## --- TODO: Write it to the files -- ##
        if param in self.uintParam.keys():
            if self._set_uint_param(param, value) == SUCCESS:
                print "{Param} Successfully Set to {Value}".format(Param=param,
                                                                   Value=value)
            else:
                print self._set_uint_param(param, value)
        elif param in self.boolParam.keys():
            if self._set_bool_param(param, value) == SUCCESS:
                print "{Param} Successfully Set to {Value}".format(Param=param,
                                                                   Value=value)
                
        elif param in self.enumParam.keys():
            if self._set_enum_param(param, value) == SUCCESS:
                print "{Param} Successfully Set to {Value}".format(Param=param,
                                                                   Value=value)
    def capture_raw(self, height, width,
                    form, compression,
                    filename):
        print self._capture_raw(height, width, form,
                                compression, filename)
                
if __name__ == '__main__':
    ec = ECapture()
    ec._readConfigFiles()
    cam_index = ec.detect_camera()
    ec.get_camera(cam_index)

    ec.read_parameter('Exposure')
    ec.set_parameter('Exposure', 1200)
    ec.read_parameter('Width')
    ec.set_parameter('Width', 1044)

    ec.read_parameter('OffsetX')
    ec.set_parameter('OffsetX', 256)

    ec.read_parameter('Height')
    ec.set_parameter('Height', 1024)

    ec.read_parameter('OffsetY')
    ec.set_parameter('OffsetY', 256)

    ec.capture_raw(height= 1024, width= 1044,
                   form="Mono8", compression="RAW",
                   filename="test.raw")
    ec.release_camera()
