/*Author: Mohit Sharma
Date: Oct 30 2015
Version: Development
NYU CUSP 2015
*/
/*
#ifndef _CLOCK_T_DEFINED
typedef long clock_t
#define _CLOCK_T_DEFINED
#endif
*/

#include <stdio.h>
#include <string.h>
#include <boost/python.hpp>
#include <EmergentCameraAPIs.h>
#include <EvtParamAttribute.h>
#include <gigevisiondeviceinfo.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <EmergentFrameSave.h>
#include <time.h>
#include <chrono>
#include <unistd.h>
#include <boost/python/numpy.hpp>
//try

using namespace std;
using namespace Emergent;
namespace bp=boost::python;
namespace np = boost::python::numpy;

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

//int currenttime()
//{
//}

class HsCam
{
public:
  // Global Variables
  struct GigEVisionDeviceInfo deviceInfo[MAX_CAMERAS];
  CEmergentCamera camera;
  unsigned int count, camera_index;
  string ip, serial, mac, name, subnet, gateway, firmver;

  string message;  

  CEmergentFrame evtFrame, evtFrameConvert;
  unsigned int enum_count;
  char filename[100];


  void configure_defaults(CEmergentCamera* camera)
  {
    unsigned int width_max, height_max, param_val_max;
    const unsigned long enumBufferSize = 1000;
    unsigned long enumBufferSizeReturn = 0;
    char enumBuffer[enumBufferSize];
    
    //Order is important as param max/mins get updated.
    
    EVT_CameraGetEnumParamRange(camera, "PixelFormat", enumBuffer, enumBufferSize, &enumBufferSizeReturn);
    char* enumMember = strtok_s(enumBuffer, ",", &next_token);
    EVT_CameraSetEnumParam(camera,      "PixelFormat", enumMember);
    
    EVT_CameraSetUInt32Param(camera,    "FrameRate", 30);
    
    EVT_CameraSetUInt32Param(camera,    "OffsetX", 0);
    EVT_CameraSetUInt32Param(camera,    "OffsetY", 0);
    
    EVT_CameraGetUInt32ParamMax(camera, "Width", &width_max);
    //EVT_CameraSetUInt32Param(camera,    "Width", 1044);
    
    EVT_CameraGetUInt32ParamMax(camera, "Height", &height_max);
    //EVT_CameraSetUInt32Param(camera,    "Height", 1024);
    
    EVT_CameraSetEnumParam(camera,      "AcquisitionMode",        "Continuous");
    EVT_CameraSetUInt32Param(camera,    "AcquisitionFrameCount",  1);
    EVT_CameraSetEnumParam(camera,      "TriggerSelector",        "AcquisitionStart");
    EVT_CameraSetEnumParam(camera,      "TriggerMode",            "Off");
    EVT_CameraSetEnumParam(camera,      "TriggerSource",          "Software");
    EVT_CameraSetEnumParam(camera,      "BufferMode",             "Off");
    EVT_CameraSetEnumParam(camera,      "SubSample",             "SUBS_2x2");
    
    EVT_CameraSetUInt32Param(camera,    "BufferNum",              0);
    
    EVT_CameraGetUInt32ParamMax(camera, "GevSCPSPacketSize", &param_val_max);
    EVT_CameraSetUInt32Param(camera,    "GevSCPSPacketSize", param_val_max);
    
    EVT_CameraSetUInt32Param(camera,    "Gain", 256);
    EVT_CameraSetUInt32Param(camera,    "Offset", 0);
    
    EVT_CameraSetBoolParam(camera,      "LUTEnable", false);
    EVT_CameraSetBoolParam(camera,      "AutoGain", false);
  }                                                

  
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
	//cout << right << setw(_cursor_posn + 11 - strlen(message.c_str()));
	//cout << COLOR_GREEN "[OK]" COLOR_RESET << endl;
	//cout << left << "Camera " << name.c_str() << " acquired successfully" << endl;
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
    /*
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
    */
    return result;
  }
  
 
  int release_camera()
  {
    message = "Releasing Camera";
    int result;
    cout << left << message;
    result = EVT_CameraClose(&camera);
    //printf("%d \n", result);
    // It is either 0 or non zero.

    /*if (result == 0)
      {
	cout <<right << setw(_cursor_posn + 11 - strlen(message.c_str()));
	cout << COLOR_GREEN "[OK]" COLOR_RESET << endl;
        cout << left << name.c_str() << "Camera Released Successfully." << endl;
      }
    
      if (result != 0)
      {
	cout << right << setw(_cursor_posn + 11 - strlen(message.c_str()));
	cout << COLOR_RED "[FAILED]" COLOR_RESET << endl;
	cout << left << "Camera" << name.c_str() << "Locked. Close any process using the camera and try again"<< endl;
      }
    */
    return result;
  }

  int detect_camera()
  {
    unsigned int listcam_buf_size = MAX_CAMERAS;
    /* Count all the Emergent Cameras regardless of their types
     * This is just for enumeration. We will only work with the
     * first HS Camera.
     */
    message = "Detecting Camera";
    cout << left << message.c_str();
    EVT_ListDevices(deviceInfo, &listcam_buf_size, &count);
    /*if (count > 0)
      {
	cout <<right << setw(_cursor_posn + 11 - strlen(message.c_str()));
	cout << COLOR_GREEN "[OK]" COLOR_RESET << endl;
      }
    else
      {
	cout <<right << setw(_cursor_posn + 11);
	cout << COLOR_RED "[FAILED]" COLOR_RESET << endl;
	}*/

    //cout << left << "Number of Cameras: " << count << endl;

    for(camera_index=0; camera_index<count; camera_index++)
      {
	//cout << left << "Camera index: ";
	//cout << COLOR_BLUE << camera_index << COLOR_RESET << endl;
	// Check if the detected camera belongs to type HS.
	if (strncmp(deviceInfo[camera_index].modelName, "HS", 2) == 0)
	  {
	    //Print the name of the camera
	    cout << endl;
	    cout << deviceInfo[camera_index].manufacturerName << endl <<
	      deviceInfo[camera_index].modelName << endl <<
	      deviceInfo[camera_index].currentIp << endl << 
	      deviceInfo[camera_index].macAddress << endl;	
	  }
	else
	  {
	    cout << left << "No Emergent Cameras Detected. " COLOR_RESET;
	  }
      }
    return camera_index-1;
  }
  
  /*
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
    // Return list of [MAX, MIN, INC] values for a
     / particular paramter string
     //
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
  */
  
  bp::list uint_param_range(const char* param)
  {
    // Return list of [MAX, MIN, INC] values for a
    // particular paramter string
    //
    bp::list result;
    size_t size = 3;
    
    vector<unsigned int> array(size);
    vector<unsigned int>::iterator it;
    
    EVT_CameraGetUInt32ParamMax(&camera, param, &array[0]);
    EVT_CameraGetUInt32ParamMin(&camera, param, &array[1]);
    EVT_CameraGetUInt32ParamInc(&camera, param, &array[2]);

    for (it= array.begin(); it != array.end(); ++it)
      {
	result.append(*it);
      }
    
    return result;
  }

  bp::list bool_param_range(const char* param)
  {
    // Return list of [MAX, MIN, INC] values for a
    // particular paramter string
    //
    bp::list result;
    bool boolresult;

    EVT_CameraGetBoolParam(&camera, param, &boolresult);
    //temp = boolresult ? "true" : "false";
    result.append(boolresult);

    return result;
  }

  bp::list enum_param_range(const char* param)
  {
    // Return list of [MAX, MIN, INC] values for a
    // particular paramter string
    //
    bp::list result;

    const unsigned long enumBufferSize = 1000;
    unsigned long enumBufferSizeReturn = 0;
    char enumBuffer[enumBufferSize];
  
    EVT_CameraGetEnumParamRange(&camera, param, enumBuffer, enumBufferSize, &enumBufferSizeReturn);
    char* buff = strtok_s(enumBuffer, ",", &next_token);
    while(buff != NULL)
      {
	result.append(string(buff));
	buff = strtok (NULL, ",");
      }

    return result;
  }

  int set_uint_param(const char* param, int val)
  {
    int result;
    result = EVT_CameraSetUInt32Param(&camera, param, val);
    return result;
  }

  int set_bool_param(const char* param, bool val)
  {
    int result;
    result = EVT_CameraSetBoolParam(&camera, param, val);
    return result;
  }

  int set_enum_param(const char* param, const char* val)
  {
    int result;
    result = EVT_CameraSetEnumParam(&camera, param, val);
    return result;
  }

  int open_cam_stream()
  {
    int result;
    result = EVT_CameraOpenStream(&camera);
    /*if (result != 0)
      {
	cout << "Cannot open Camera stream " << result << endl;
	}*/
    return result;
  }

  int close_cam_stream()
  {
    int result;
    result = EVT_CameraCloseStream(&camera);
    /*if (result != 0)
      {
	cout << "Cannot close Camera stream " << result << endl;
	}*/
    return result;
  }
  
  
  int capture_image(int height, int width, const char* format, const char* compression)
  {
    int result;
    //cout << height << endl << width << endl << format << endl << compression << endl;
    
    // Memory Allocation for capturing frames
    evtFrame.size_x = width;
    evtFrame.size_y = height;
    //evtFrame.pixel_type = GVSP_PIX_RGB10_PACKED;
    //evtFrame.pixel_type = GVSP_PIX_MONO8;
    evtFrame.pixel_type = GVSP_PIX_RGB8_PACKED;
    // Queuing the frame
    result = EVT_AllocateFrameBuffer(&camera, &evtFrame, EVT_FRAME_BUFFER_ZERO_COPY);
    if (result)
      {
	cout << "Frame Grab Allocate Frame Buffer Error = " << result << endl;
      }
    
    // Memory allocation for converted frames
    evtFrameConvert.size_x = width;
    evtFrameConvert.size_y = height;
    //evtFrameConvert.pixel_type = GVSP_PIX_RGB10_PACKED;
    //evtFrameConvert.pixel_type = GVSP_PIX_MONO8;
    evtFrameConvert.pixel_type = GVSP_PIX_RGB8_PACKED;
    evtFrameConvert.convertColor = EVT_COLOR_CONVERT_BILINEAR;
    evtFrameConvert.convertBitDepth = EVT_CONVERT_8BIT;
    
    result = EVT_AllocateFrameBuffer(&camera, &evtFrameConvert, EVT_FRAME_BUFFER_DEFAULT);
    if (result)
      {
	cout << "Frame Convert Allocate Frame Buffer Error = " << result << endl;
      }
    
    // Set the pixel format
    EVT_CameraSetEnumParam(&camera, "PixelFormat", format);

    //cout << "Grabbing a single Frame" << endl;

    // Queueing the Frames
    result = EVT_CameraQueueFrame(&camera, &evtFrame);
    if (result)
      {
	cout << "EVT_CameraQueueFrame Error = " << result << endl; 
      }

    //cout << "capture_image: " << result << endl;
    return result;
  }

  void capture_raw(int height, int width, const char* format,
		  const char* compression, const char* filename)
  {
    configure_defaults(&camera);
    int result;
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    //try time
   // boost::chrono::high_resolution_clock::time_point start = boost::chrono::high_resoltuion_clock::now()'

    //clock_t temptime;
    //temptime = (double)clock();
    //auto time0 = Time::now();
    //cout<<"time_before_capture_raw: ";
    //cout<< time0 <<endl;
    //sleep(2);
    //temptime = (double)clock();
    //cout<<"time_after_sleep_2000ms: "<<temptime << endl;
    /* timeval curTime;
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 1000;
    char buffer [80];
    strftime(buffer, 80 "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));
    
    char currentTime[84] = "";
    sprintf(currentTime, "%s:%d", buffer, milli);
    printf("current time: %s \n", currentTime);
    */
    //temptime = (double)clock();
    //cout<<"time_before_conf_default: "<<temptime<<endl;

    
    //temptime = (double)clock();
    // cout<<"time_after_conf_default: "<<temptime<<endl;

    // Lets now grab a single frame
    unsigned int frames_recd = 0;

    result = capture_image(height, width, format, compression);
    if (result != 0)
      {
	cout << "Error Capturing Frame" << endl;
      }
    auto time0 = Time::now();
    // Tell Camera to start streaming
    result = EVT_CameraExecuteCommand(&camera, "AcquisitionStart");
    if (result != 0)
      {
	//cout << "Camera Acquisition Error = " << result << endl;
      }

    // Receive Frame
    result = EVT_CameraGetFrame(&camera, &evtFrame, EVT_INFINITE);
    if (result != 0)
      {
	cout << "Error Receiving Frames = " << result << endl;
      }
    else
      {
	frames_recd++;
      }

    // Tell Camera to stop streaming
    EVT_CameraExecuteCommand(&camera, "AcquisitionStop");


    // result = capture_image(height, width, format, compression);
       
    //temptime = (double)clock();
    //cout<<"time_after_capure_image: "<<temptime<<endl;

    //if (result != 0)
    //  {
    //	cout << "Error Capturing Frame" << endl;
    //  }

    //result = EVT_FrameSave(&evtFrame, filename, EVT_FILETYPE_RAW, EVT_ALIGN_NONE);
    result = 0;
    if (result)
      {
	cout << "Frame Save Error = " << result << endl;
      }
    //EVT_FrameSave(&evtFrame, filename, EVT_FILETYPE_RAW, EVT_ALIGN_NONE);
    //cout << "Done!" << endl;


    // ---- Frame manipulation

    unsigned char *frame_img_ptr = new unsigned char;
    int nelems = 4096*3072;
    int *array2 = new int[nelems];
    
    frame_img_ptr = evtFrame.imagePtr;
    cout << " ------ " << endl;
    cout << frame_img_ptr[4000] << endl;
    cout << " ------ " << endl;

    /** Contents of evtFrame
    cout << "Size X: ";
    cout << evtFrame.size_x << endl;
    cout << "Size Y: ";
    cout << evtFrame.size_y << endl;
    cout << "Convert Color: ";
    cout << evtFrame.convertColor << endl;
    cout << "Convert Bit Depth: ";
    cout << evtFrame.convertBitDepth << endl;
    **/

    //copy(frame_img_ptr, frame_img_ptr+nelems, array2);
    
    //for (int i = 0; i < nelems; i+=2) {
    //  array2[i] = frame_img_ptr[i];
    //}

    memcpy(array2, frame_img_ptr, sizeof(frame_img_ptr));
    
    //for (int i = 0; i < nelems; i++) {
    //  cout << array2[i];
    //}
    /*
    np::dtype dt = np::dtype::get_builtin<int>();
    bp::tuple shape = bp::make_tuple(nelems);
    bp::tuple stride = bp::make_tuple(sizeof(int));
    bp::object own;

    np::ndarray imgarr = np::from_data(array2, dt, shape, stride, own);
    //cout << "Single dimensional array ::" << endl
    // << bp::extract<char const *>(bp::str(imgarr)) << endl;
    */
    cout << "Printing 1000th element: ";
    cout << array2[1000] << endl;
    // ----

    
    //try
    //temptime = clock();
    auto time1 = Time::now();
    fsec fs = time1 - time0;
    ms d = std::chrono::duration_cast<ms>(fs);
    cout<<"time_after_capture_raw: ";
    //cout<<fs.count()<<endl;
    cout<<"in ms: ";
    cout<<d.count()<<endl;
    // boost::chrono::nanoseconds ns = boost::chrono::high_resolution_clock::now() = start;
    //auto val = ns.count()'
    //cout<<"boost time: "<<val<<endl;
    // Teardown buffer and camera stream
    EVT_ReleaseFrameBuffer(&camera, &evtFrame);
    EVT_ReleaseFrameBuffer(&camera, &evtFrameConvert);
    

  }

};


BOOST_PYTHON_MODULE(CEmergent)
{
  // docstring_options(bool user_defined, bool py_signatures, bool cpp_signatures);
  bp::docstring_options local_docstring_options(true, true, false);
  bp::class_<HsCam>("HsCam", "Wrapper Class for Emergent HS cameras")
    .def_readonly("name", &HsCam::name, "Manufacturer specified Name of the camera")
    .def_readonly("ip", &HsCam::ip, "IP Address obtained by the camera")
    .def_readonly("mac", &HsCam::mac, "MAC Address of the camera")
    .def_readonly("serial", &HsCam::serial, "Manufacturer specified Serial number of the camera")
    .def_readonly("subnet", &HsCam::subnet, "Subnet mast configured on the camera")
    .def_readonly("gateway", &HsCam::gateway, "Gateway address")
    .def_readonly("firmver", &HsCam::firmver, "Current Firmware version on the camera")

    .def("_detect_camera", &HsCam::detect_camera, "Detect the HS camera connected on the LAN")
    .def("_get_camera", &HsCam::get_camera, (bp::arg("camera_index")),
	 "Obtain a lock on the camera")
    .def("_release_camera", &HsCam::release_camera, "Release the lock on the camera")

    // Get Parameters
    .def("_uint_param_range", &HsCam::uint_param_range, (bp::arg("param")), 
	 "Get value for the uint parameter passed in argument. "
	 "\n \t for list of paramters refer " HELP_FILE)
    .def("_bool_param_range", &HsCam::bool_param_range, (bp::arg("param")), 
	 "Get current value for the bool parameter passed in argument. "
	 "\n \t for list of paramters refer " HELP_FILE)
    .def("_enum_param_range", &HsCam::enum_param_range, (bp::arg("param")), 
	 "Get value for the enum parameter passed in argument. "
	 "\n \t for list of paramters refer " HELP_FILE)
    
    
    // set parameters
    .def("_set_uint_param", &HsCam:: set_uint_param, (bp::arg("param")), (bp::arg("val")),
	 "Set value for the parameter passed in argument. "
	 "\n \t for list of paramters refer " HELP_FILE)
    
    .def("_set_bool_param", &HsCam:: set_bool_param, (bp::arg("param")), (bp::arg("val")),
	 "Set value for the parameter passed in argument. "
	 "\n \t for list of paramters refer " HELP_FILE)

    .def("_set_enum_param", &HsCam:: set_enum_param, (bp::arg("param")), (bp::arg("val")),
	 "Set value for the parameter passed in argument. "
	 "\n \t for list of paramters refer " HELP_FILE)


    // Open and Close cam streams
    .def("_open_cam_stream", &HsCam::open_cam_stream, "Open the Camera Stream for capturing images")
    
    .def("_close_cam_stream", &HsCam::close_cam_stream, "Close the Camera Stream")

    
    // Capture image
    .def("_capture_raw", &HsCam:: capture_raw, ((bp::arg("height")), (bp::arg("width")),
						(bp::arg("format")), (bp::arg("compression")),
						(bp::arg("filename"))),
	 "Capture image in raw format. Parameters required: \n\t height \n\t width \n\t format \n\t"
	 "compression \n\t filename \n \t for more info refer " HELP_FILE)

    ;
}
