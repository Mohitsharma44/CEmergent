/* 
Author: Mohit Sharma
Date: Oct 30 2015
Versions: Development
NYU CUSP 2015
*/

#include <stdio.h>
#include <string.h>
#include <EmergentCameraAPIs.h>
#include <EvtParamAttribute.h>
#include <gigevisiondeviceinfo.h>
#include <iostream>
#include <boost/python.hpp>

using namespace std;
using namespace Emergent;

#define MAX_CAMERAS 10

// Global Variables
struct GigEVisionDeviceInfo deviceInfo[MAX_CAMERAS];
unsigned int listcam_buf_size = MAX_CAMERAS;

bool camera_info()
{
  unsigned int count, camera_index;
  char camera_model[100];
  EVT_ListDevices(deviceInfo, &listcam_buf_size, &count);
  printf("Number of Cameras: %d \n", count);

  for(camera_index=0; camera_index<count;camera_index++)
    {
      strcpy_s(camera_model, deviceInfo[camera_index].modelName);
      printf("Detected Camera: %s \n", camera_model);
      //camera_model[2] = '\0';
    }
  if(count > 0)
    {
      return true;
    }
  else
    {
      return false;
    }
}


BOOST_PYTHON_MODULE(CEmergent)
{
  namespace bp=boost::python;
  bp::def("camera_info", camera_info);
}
