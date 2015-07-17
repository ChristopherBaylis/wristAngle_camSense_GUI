
[EnableStream[s]][EnableStreams]
>If the application specifies more than 1 stream in the stream descriptor, the PXC[M]SenseManager interface delivers all streams synchronized when invoking the `OnImageSamples` callback function.

[Capturing Aligned/Unaligned Color and Depth Streams][]
>`// This function blocks until any sample is ready`
    `if (sm->AcquireFrame(false)<PXC_STATUS_NO_ERROR) break;`

[Intel® RealSense™ Data Ranges][]
>0.2 – 1.2 meter range (Specific algorithms may have different range and accuracy)

>>Hand Segmentation 	20-80 cm

>>Hand Tracking 	20-60 cm

[setRealtime][]
>Although the real time mode is good for visually checking if recording is done right, playing back a file in the real time mode is not repeatable, depending largely on the current system workload. **If repeatability is needed, use realtime=false.**

[Hand Tracking and Recognition With OpenCV][]

[EnableStreams]: https://software.intel.com/sites/landingpage/realsense/camera-sdk/v1.1/documentation/html/enablestreams_pxcsensemanager.html?zoom_highlightsub=stream
[Capturing Aligned/Unaligned Color and Depth Streams]: https://software.intel.com/sites/landingpage/realsense/camera-sdk/v1.1/documentation/html/manuals_color_and_depth_picture_alignment.html?zoom_highlightsub=stream
[Intel® RealSense™ Data Ranges]: https://software.intel.com/en-us/articles/intel-realsense-data-ranges
[setRealtime]: https://software.intel.com/sites/landingpage/realsense/camera-sdk/v1.1/documentation/html/setrealtime_pxccapturemanager.html
[Hand Tracking and Recognition With OpenCV]: http://simena86.github.io/blog/2013/08/12/hand-tracking-and-recognition-with-opencv/


OpenCV settup possible errors
"The application was unable to start correctly (0xc000007b). Click OK to close the application." 
"the program can't start because msvcp110d.dll is missing"
"error LNK1112: module machine type 'x64' conflicts with target machine type 'X86'"
"error LNK1104: cannot open file 'opencv_calib3d249.lib'"
"error C2871: 'cv' : a namespace with this name does not exist	"
"the program can't start because opencv_core2411d.dll is missing"