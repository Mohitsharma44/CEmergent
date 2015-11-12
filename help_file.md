## Help File for CEmergent Commands
---------

This is a minimal python wrapper for application of **Emergent** HS type cameras at NYU **C**USP.
The library has been tested with Emergent HS-12000C but should work with other HS cameras too. The library has been 
built using SDK version 2.3.2 running on Ubuntu 12.04 with kernel 3.11.24-generic.

The documentation will provide the method name, description and parameters that can be passed to the method.
The methods in the wrapper also includes a short docstring explaining the methods.

### Available Methods and attributes in CEmergent:

#### Class:
* `HsCam`

#### Methods:
* `detect_camera`:
Arguments: `None`

This method will enumerate all the cameras connected on the myricom network card interface and print the index value
of the camera, Model name, MAC Address and IP Address.
This method does not acquire lock on the camera.


*`get_camera`:
Arguments: `int camera_index`

This method will obtain a lock on the camera whose index value is passed. This method will also querry the camera for
its Model name, Serial Number, Mac Address, IP Address, Firmware version, subnet, gateway and store the value as string
inside corresponding attributes. (Refer: Attributes)


*`release_camera`:
Arguments: `None`

This method will release the lock on the camera that was most recently locked.
> Note: If using multiple cameras, make sure to release lock first before acquiring other camera.

*`get_param`:
Arguments: `string parameter_name`

This method will return a list of MAX, MIN and INC (maximum, minimum and quantization/incremental) value of the parameter passed as string.
```
Width: The horizontal size of the ROI
Height: The vertical size of the ROI
OffsetX: The horizonal offset of the ROI
OffsetY: The vertical offset of the ROI
Exposure: Camera Exposure in microseconds
FrameRate: Frame Rate of the camera in fps
Gain: Gain applied by camera before transmission to controller
AutoGainIGain: AutoGain Rate
GevSCPSPacketSize: Ethernet MTU size for transmission to controller

```