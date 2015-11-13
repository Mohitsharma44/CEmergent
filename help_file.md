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


* `get_camera`:
Arguments: `int camera_index`

This method will obtain a lock on the camera whose index value is passed. This method will also querry the camera for
its Model name, Serial Number, Mac Address, IP Address, Firmware version, subnet, gateway and store the value as string
inside corresponding attributes. (Refer: Attributes)


* `release_camera`:
Arguments: `None`

This method will release the lock on the camera that was most recently locked.
> Note: If using multiple cameras, make sure to release lock first before acquiring other camera.

* `get_param`:
Arguments: `string parameter_name`

**`Width`**: The horizontal size of the ROI

**`Height`**: The vertical size of the ROI

**`OffsetX`**: The horizonal offset of the ROI

**`OffsetY`**: The vertical offset of the ROI

**`Exposure`**: Camera Exposure in microseconds

**`FrameRate`**: Frame Rate of the camera in fps

**`Gain`**: Gain applied by camera before transmission to controller

**`AutoGainIGain`**: AutoGain Rate

**`GevSCPSPacketSize`**: Ethernet MTU size for transmission to controller

**`AutoGainSet`**: AutoGain set point

**`AutoGainIGain`**: AutoGain rate

**`WB_R_GAIN_Value`**: Red Channel white balance gain

**`WB_GR_GAIN_Value`**: Green Channel white balance gain

**`WB_B_GAIN_Value`**: Blue Channel white balance gain

> The above parameters will return a list of MAX, MIN and INC (maximum, minimum and quantization/incremental) value.

**`AutoGain`**: Enable/ Disable AutoGain. Use AutoGainSet and AutoGainIGain to program set point and integral gain (auto gain rate) respectively

**`LUTEnable`**: Enable/ Disable LUT. (LUT functionality is not implemented in this wrapper. Leave it disabled.)

**`HDREnable`**: Enable/ Diable HDR mode. (HDR functionality is not implemented in this wrapper. Leave it disabled.)

**`WB_Enable`**: Enable/ Disable Auto White Balance mode.

**`WB_Hold`**: Auto White Balance Hold mode. Once an acceptable white balance is achieved through the auto white balance function
one can enable this to hold the current white balance gains. Disable this to continue the auto white balance function

> The above parameters will return a list of either "true" or "false" strings.

**`PixelFormat`**: Pixel output format for the camera

**`SubSample`**: Same Field of View but skip pixels for lower resolution image

**`AcquisitionMode`**: Continuous/ Multiframe

**`TriggerSelector`**: always set to Acquisition start

**`TriggerMode`**: Enable/ Disable non-continuous trigger modes

**`TriggerSource`*: Trigger Sources

**`BufferMode`**: Off -- Normal operation i.e writing to frame buffer.

> The above parameters will return a list of supported modes.