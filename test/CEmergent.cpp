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
#include <fstream>
#include <boost/python.hpp>
#include <vector>
#include <algorithm>

using namespace std;
using namespace Emergent;
namespace bp=boost::python;

#define MAX_CAMERAS 10
#define HELP_FILE "help_file.md"

// ANSI escape codes to show colors on terminal
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

class HsCam
{
public:
  // Global Variables
  struct GigEVisionDeviceInfo deviceInfo[MAX_CAMERAS];
  CEmergentCamera camera;
  unsigned int count, camera_index;
  string ip, serial, mac, name, subnet, gateway, firmver;
  // Basically for paramter querries. Restricting the size of
  // buffer to prevent any bug from crashing the application

  int get_camera(unsigned int cam_index = 0)
  {
    unsigned long StringSize = 256;
    unsigned long StringSizeReturn = 0;
    char StringBuffer[StringSize];
    int result;
    //printf("Camera Index: %u",camera_index);
    printf("Obtaining Camera \t \t \t \t \t");
    result = EVT_CameraOpen(&camera, &deviceInfo[cam_index]);
    //printf("%d \n", result);
    // 0 -- Success
    // 290 -- Camera not found
    // 300 -- Camera already acquired 
    if (result == 0)
      {
	name = deviceInfo[cam_index].modelName;
	printf(COLOR_GREEN "[OK]\n" COLOR_RESET);
	printf("Camera %s acquired successfully. \n", name.c_str());
	serial = deviceInfo[cam_index].serialNumber;
	mac = deviceInfo[cam_index].macAddress;
	ip = deviceInfo[cam_index].currentIp;
	subnet = deviceInfo[cam_index].currentSubnetMask;
	gateway = deviceInfo[cam_index].defaultGateway;
	/* To obtain the firmware version of the camera,
	 * we need to access the camera and use the
	 * CameraGetStringParam.
	 */
	if (EVT_CameraGetStringParam(&camera, "DeviceFirmwareVersion", StringBuffer, StringSize, &StringSizeReturn) == 0)
          {
            firmver = StringBuffer;
          }
      }
    else if (result == 290)
      {
	printf(COLOR_RED "[FAILED]\n" COLOR_RESET);
	printf("Camera could not be found.. \n" COLOR_RESET);
      }
    else if (result == 300)
      {
	printf(COLOR_YELLOW "[WARNING]\n" COLOR_RESET);
	printf("Camera already acquired. \n" COLOR_RESET);
      }
    return result;
  }
  
 
  int release_camera()
  {
    int result;
    printf("Releasing Camera %s \t \t \t \t", name.c_str());
    result = EVT_CameraClose(&camera);
    //printf("%d \n", result);
    // It is either 0 or non zero.
    if (result != 0)
      {
	printf(COLOR_RED "[FAILED]\n" COLOR_RESET);
	printf("Camera %s Locked. Close any process using the camera and try again \n", name.c_str());
      }
    else
      {
	printf(COLOR_GREEN "[OK]\n" COLOR_RESET);
        printf("%s Camera Released Successfully. \n", name.c_str());
      }
    return result;
  }

  void detect_camera()
  {
    unsigned int listcam_buf_size = MAX_CAMERAS;
    /* Count all the Emergent Cameras regardless of their types
     * This is just for enumeration. We will only work with the
     * first HS Camera.
     */
    printf("Starting Network Scan \t \t \t \t \t");
    EVT_ListDevices(deviceInfo, &listcam_buf_size, &count);
    if (count > 0)
      {
	printf(COLOR_GREEN "[OK]\n" COLOR_RESET);
      }
    else
      {
	printf(COLOR_RED "[FAILED]\n" COLOR_RESET);
      }
    
    printf("Number of Cameras: %d \n", count);
    for(camera_index=0; camera_index<count; camera_index++)
      {
	printf("Camera index: ");
	printf(COLOR_BLUE "%d \n" COLOR_RESET, camera_index);
	// Check if the detected camera belongs to type HS.
	if (strncmp(deviceInfo[camera_index].modelName, "HS", 2) == 0)
	  {
	    // Print the name of the camera
	    printf("%s : %s \t %s \t %s\n",
		   deviceInfo[camera_index].manufacturerName, 
		   deviceInfo[camera_index].modelName,
		   deviceInfo[camera_index].currentIp,
		   deviceInfo[camera_index].macAddress);
	  }
	else
	  {
	    printf("No Emergent Cameras Detected. \n" COLOR_RESET);
	  }
      }
    
  }

  // Read all the configuration files and return the vectors
  const vector<string> get(string fname) const
  {
    vector<string> params;
    printf("Opening %s \t \t \t \t", fname.c_str());
    ifstream paramfile(fname.c_str());
    if(!paramfile)
      {
	printf(COLOR_RED "[FAILED]\n" COLOR_RESET);
	printf("Error Opening file.\n");
      }
    else
      {
	printf(COLOR_GREEN "[OK]\n" COLOR_RESET);
	for (string line; getline(paramfile, line);)
	  params.push_back(line);
      }
    return params;
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
    
    vector<string> stringParam = get("./stringParam.conf");

    // Read all the string parameters from stringParam.conf file
    //printf("Opening stringParamters configuration file \t \t");
    /*
    ifstream stringparameters("./stringParam.conf");
    if(!stringparameters)
      {
	printf(COLOR_RED "[FAILED]\n" COLOR_RESET);
	printf("Error Opening stringParam.conf file.\n");
      }
    else
      {
	printf(COLOR_GREEN "[OK]\n" COLOR_RESET);
	for (string line; getline(stringparameters, line);)
	  stringParam.push_back(line);
      }
    */
    
    // Check if the param exists in the file!
    
    if (find(stringParam.begin(), stringParam.end(), param) != stringParam.end())
      {
	printf("Found: %s\n", param);
	// unsigned int max_param_value, min_param_value, inc_param_value;    
	EVT_CameraGetUInt32ParamMax(&camera, param, &array[0]);
	EVT_CameraGetUInt32ParamMin(&camera, param, &array[1]);
	EVT_CameraGetUInt32ParamInc(&camera, param, &array[2]);
	
	for (it= array.begin(); it != array.end(); ++it)
	  {
	    printf("%d \n", *it);
	    result.append(*it);
	  }

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
    .def("get_camera", &HsCam::get_camera, (bp::arg("camera_index")),
	 "Obtain a lock on the camera")
    .def("release_camera", &HsCam::release_camera, "Release the lock on the camera")
    .def("param_range", &HsCam::param_range, (bp::arg("param")), 
	 "Get current value for the parameter passed in argument. "
	 "\n \t for list of paramters refer " HELP_FILE)
    ;
}
