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
#include <iomanip>
#include <fstream>
#include <boost/python.hpp>
#include <vector>
#include <algorithm>

using namespace std;
using namespace Emergent;
namespace bp=boost::python;

#define MAX_CAMERAS 10
#define HELP_FILE "help_file.md"
#define  _cursor_posn 58

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

  string message;
  
  int get_camera(unsigned int cam_index = 0)
  {
    unsigned long StringSize = 256;
    unsigned long StringSizeReturn = 0;
    char StringBuffer[StringSize];
    int result;
    message = "Obtaining Camera";
    cout << left << message;
    result = EVT_CameraOpen(&camera, &deviceInfo[cam_index]);
    //printf("%d \n", result);
    // 0 -- Success
    // 290 -- Camera not found
    // 300 -- Camera already acquired 
    if (result == 0)
      {
	name = deviceInfo[cam_index].modelName;
	cout << right << setw(_cursor_posn + 11 - strlen(message.c_str()));
	cout << COLOR_GREEN "[OK]" COLOR_RESET << endl;
	cout << left << "Camera" << name.c_str() << "acquired successfully" << endl;
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
	cout << right << setw(_cursor_posn + 11-strlen(message.c_str()));
	cout << COLOR_GREEN "[FAILED]" COLOR_RESET << endl;
	cout << left << "Camera could not be found.. " COLOR_RESET << endl;
      }
    else if (result == 300)
      {
	cout << right << setw(_cursor_posn + 11-strlen(message.c_str()));
	cout << COLOR_GREEN "[WARNING]\n" COLOR_RESET << endl;
	cout << left << "Camera already acquired." COLOR_RESET << endl;
      }
    return result;
  }
  
 
  int release_camera()
  {
    message = "Releasing Camera";
    int result;
    cout << left << message << name.c_str();
    result = EVT_CameraClose(&camera);
    //printf("%d \n", result);
    // It is either 0 or non zero.
    if (result != 0)
      {
	cout << right << setw(_cursor_posn + 11 - strlen(message.c_str()));
	cout << COLOR_RED "[FAILED]" COLOR_RESET << endl;
	cout << left << "Camera" << name.c_str() << "Locked. Close any process using the camera and try again"<< endl;
      }
    else
      {
	cout <<right << setw(_cursor_posn + 11 - strlen(message.c_str()));
	cout << COLOR_GREEN "[OK]" COLOR_RESET << endl;
        cout << left << name.c_str() << "Camera Released Successfully." << endl;
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
    message = "Detecting Camera";
    cout << left << message.c_str();
    EVT_ListDevices(deviceInfo, &listcam_buf_size, &count);
    if (count > 0)
      {
	cout <<right << setw(_cursor_posn + 11 - strlen(message.c_str()));
	cout << COLOR_GREEN "[OK]" COLOR_RESET << endl;
      }
    else
      {
	cout <<right << setw(_cursor_posn + 11);
	cout << COLOR_RED "[FAILED]" COLOR_RESET << endl;
      }

    cout << left << "Number of Cameras: " << count << endl;

    for(camera_index=0; camera_index<count; camera_index++)
      {
	cout << left << "Camera index: ";
	cout << COLOR_BLUE << camera_index << COLOR_RESET << endl;
	// Check if the detected camera belongs to type HS.
	if (strncmp(deviceInfo[camera_index].modelName, "HS", 2) == 0)
	  {
	    // Print the name of the camera
	    cout << deviceInfo[camera_index].manufacturerName <<
	      deviceInfo[camera_index].modelName <<
	      deviceInfo[camera_index].currentIp <<
	      deviceInfo[camera_index].macAddress << endl;	
	  }
	else
	  {
	    cout << left << "No Emergent Cameras Detected. " COLOR_RESET;
	  }
      }
    
  }

  // Read the configuration files and return the vectors
  const vector<string> fileToVector(string fname) const
  {
    string message = "Opening";
    vector<string> params;
    ifstream paramfile(fname.c_str());
    if(!paramfile)
      {
	cout << left << message.c_str() << fname.c_str();
	cout << right << setw(_cursor_posn - strlen(message.c_str()));
	cout << COLOR_RED "[FAILED]" COLOR_RESET << endl;
	cout << left << "Error Opening file." << endl;
      }
    else
      {
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
    bool boolresult;
    string temp;
    vector<unsigned int> array(size);
    vector<unsigned int>::iterator it;
    
    vector<string> stringParam = fileToVector("./uint32Param.conf");
    vector<string> boolParam = fileToVector("./boolParam.conf");
    vector<string> enumParam = fileToVector("./enumParam.conf");

    const unsigned long enumBufferSize = 1000;
    unsigned long enumBufferSizeReturn = 0;
    char enumBuffer[enumBufferSize];
    cout << left << "Looking up " << param;
    // Find the parameter in the configuration files
    if (find(stringParam.begin(), stringParam.end(), param) != stringParam.end())
      {
	cout << right << setw(_cursor_posn-strlen(param));
	cout << COLOR_GREEN "[OK]" COLOR_RESET << endl;
	// unsigned int max_param_value, min_param_value, inc_param_value;    
	EVT_CameraGetUInt32ParamMax(&camera, param, &array[0]);
	EVT_CameraGetUInt32ParamMin(&camera, param, &array[1]);
	EVT_CameraGetUInt32ParamInc(&camera, param, &array[2]);
	
	for (it= array.begin(); it != array.end(); ++it)
	  {
	    result.append(*it);
	  }
      }

    else if (find(boolParam.begin(), boolParam.end(), param) != boolParam.end())
      {
	cout << right << setw(_cursor_posn-strlen(param));
	cout << COLOR_GREEN "[OK]" COLOR_RESET << endl;
	EVT_CameraGetBoolParam(&camera, param, &boolresult);
	temp = boolresult ? "true" : "false";
	result.append(temp);
      }

    else if (find(enumParam.begin(), enumParam.end(), param) != enumParam.end())
      {
	cout << right << setw(_cursor_posn-strlen(param));
	cout << COLOR_GREEN "[OK]" COLOR_RESET << endl;
	EVT_CameraGetEnumParamRange(&camera, param, enumBuffer, enumBufferSize, &enumBufferSizeReturn);
	char* buff = strtok_s(enumBuffer, ",", &next_token);
	while(buff != NULL)
	  {
	    result.append(string(buff));
	    buff = strtok (NULL, ",");
	  }
      }
    else
      {
	cout << right << setw(_cursor_posn);
	cout << COLOR_RED "[FAILED]" COLOR_RESET << endl;
	cout << left << COLOR_YELLOW "Invalid Parameter Passed" COLOR_RESET << endl;
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
