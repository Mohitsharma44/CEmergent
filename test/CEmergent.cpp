/* 
Author: Mohit Sharma
Date: Oct 30 2015
Version: Development
NYU CUSP 2015
*/

#include <stdio.h>
#include <string.h>
#include <EmergentCameraAPIs.h>
#include <EvtParamAttribute.h>
#include <gigevisiondeviceinfo.h>
#include <iostream>
#include <boost/python.hpp>
#include <vector>

using namespace std;
using namespace Emergent;
namespace bp=boost::python;

#define MAX_CAMERAS 10
#define HELP_FILE "help_file.md"

class HsCam
{
public:
  // Global Variables
  struct GigEVisionDeviceInfo deviceInfo[MAX_CAMERAS];
  CEmergentCamera camera;
  unsigned int count, camera_index;
  string ip, serial, mac, name, subnet, gateway, firmver;
  int get_camera()
  {
    int result;
    printf("Obtaining Camera \n");
    result = EVT_CameraOpen(&camera, &deviceInfo[camera_index]);
    //printf("%d \n", result);
    // 0 -- Success
    // 290 -- Camera not found
    // 300 -- Camera already acquired 
    if (result == 0)
      {
	printf("Camera acquired successfully. \n");
      }
    else if (result == 290)
      {
	printf("Camera could not be found.. \n");
      }
    else if (result == 300)
      {
	printf("Camera already acquired. \n");
      }
    return result;
  }
  
 
  int release_camera()
  {
    int result;
    printf("Releasing Camera \n");
    result = EVT_CameraClose(&camera);
    //printf("%d \n", result);
    // It is either 0 or non zero.
    if (result != 0)
      {
	printf("Camera Locked. Close any process using the camera and try again \n");
      }
    else
      {
        printf("Camera Released Successfully. \n");
      }
    return result;
  }

  int detect_camera()
  {
    unsigned int listcam_buf_size = MAX_CAMERAS;
    const unsigned long StringSize = 256;
    unsigned long StringSizeReturn = 0;
    char StringBuffer[StringSize];
    int test;
    /* Count all the Emergent Cameras regardless of their types
     * This is just for enumeration. We will only work with the
     * first HS Camera.
     */
    EVT_ListDevices(deviceInfo, &listcam_buf_size, &count);
    printf("Number of Cameras: %d \n", count);
    for(camera_index=0; camera_index<count; camera_index++)
      {
	printf("Camera index: %d \n", camera_index);
	// Check if the detected camera belongs to type HS.
	if (strncmp(deviceInfo[camera_index].modelName, "HS", 2) == 0)
	  {
	    // Print the name of the camera
	    printf("Detected: %s : %s\n", deviceInfo[camera_index].manufacturerName, 
					   deviceInfo[camera_index].modelName);
	    name = deviceInfo[camera_index].modelName;
	    serial = deviceInfo[camera_index].serialNumber;
	    mac = deviceInfo[camera_index].macAddress;
	    ip = deviceInfo[camera_index].currentIp;
	    subnet = deviceInfo[camera_index].currentSubnetMask;
	    gateway = deviceInfo[camera_index].defaultGateway;

	    // grab the first HS type camera and break
	    break;
	  }
	else
	  {
	    printf("No Emergent Cameras Detected. \n");
	  }
      }
    /* To obtain the firmware version of the camera,
     * we need to access the camera and use the
     * CameraGetStringParam.
     */
    test = get_camera();
    if (test == 0)
      {
	if (EVT_CameraGetStringParam(&camera, "DeviceFirmwareVersion", StringBuffer, StringSize, &StringSizeReturn) == 0)
	  {
	    firmver = StringBuffer;
	  }
      }
    release_camera();
    return camera_index;
  }
  
  bp::list param_range(const char* param)
  {
    /* Return list of [MAX, MIN, INC] values for a
     * particular paramter string
     */
    bp::list result;
    size_t size = 3;
    vector<unsigned int> array(size);
    vector<unsigned int>::iterator it;
    
    //unsigned int max_param_value, min_param_value, inc_param_value;
    EVT_CameraGetUInt32ParamMax(&camera, param, &array[0]);
    EVT_CameraGetUInt32ParamMin(&camera, param, &array[1]);
    EVT_CameraGetUInt32ParamInc(&camera, param, &array[2]);
    
    for (it= array.begin(); it != array.end(); ++it)
      {
        printf("%d \n", *it);
        result.append(*it);
      }
    return result;
  }


};



BOOST_PYTHON_MODULE(CEmergent)
{
  // docstring_options(bool user_defined, bool py_signatures, bool cpp_signatures);
  bp::docstring_options local_docstring_options(true, true, false);
  bp::class_<HsCam>("HsCam", "Wrapper Class for Emergent HS cameras")
    .def("detect_camera", &HsCam::detect_camera, "Detect the HS camera connected on the LAN")
    .def_readonly("name", &HsCam::name, "Manufacturer specified Name of the camera")
    .def_readonly("ip", &HsCam::ip, "IP Address obtained by the camera")
    .def_readonly("mac", &HsCam::mac, "MAC Address of the camera")
    .def_readonly("serial", &HsCam::serial, "Manufacturer specified Serial number of the camera")
    .def_readonly("subnet", &HsCam::subnet, "Subnet mast configured on the camera")
    .def_readonly("gateway", &HsCam::gateway, "Gateway address")
    .def_readonly("firmver", &HsCam::firmver, "Current Firmware version on the camera")
    .def("get_camera", &HsCam::get_camera, "Obtain a lock on the camera")
    .def("release_camera", &HsCam::release_camera, "Release the lock on the camera")
    .def("param_range", &HsCam::param_range, (bp::arg("param")), 
	 "Get current value for the parameter passed in argument. "
	 "\n \t for list of paramters refer " HELP_FILE)
    ;
}
