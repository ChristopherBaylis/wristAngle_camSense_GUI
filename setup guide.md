This project leverages the follwing technologies

* Intel RealSense
* Qt

Intel RealSense SDK R3 V5.0.3.7777 (5.0.3.187777) was used and had the following system requirements:

* Hardware requirements:
    * 4th generation Intel® Core™ processors based on the Intel microarchitecture code name Haswell
    * 8GB free hard disk space
    * Intel® RealSense™ 3D camera (required to connect to a USB* 3 port)
* Software requirements:
    * Microsoft* Windows 8.1 OS 64-bit
    * Microsoft Visual Studio* 2010-2013 with the latest service pack

This project was developed **June-Jully 2015** using:

* Development Environment
    * Intel RealSense SDK R3 V5.0.3.7777 (5.0.3.187777)
    * Visual Studio 2013
    * Qt 5.4.2 with VS Add-In 1.2.4
* Computer (Acer travelmate p645 v4da2)
    * Intel(R) Core i5-4210U CPU @ 1.70GHz 2.40 GHz
    * Windows 8.1 Enterprise (6.2.9200)
* Camera
    * Intel® RealSense™Develper Kit Camera (F200) (Creative VF0800)
    * "Intel RealSense Depth Camera Manager". F200 DCM V1.2 Build 1.3.20.55679

You can see more information on the development machines specs in the output of the RSSDK Information Viewer output file DevelopmentMachin_SDKViewerSpecs.TXT

**Important note:** RSSDK R3 appears to be relativly mature with many powerfull functions and abillities. However things appear to be changing at a fairly regular interval. This docuemt is accurate within the context of the above specification and at the date of writing.

#Quickstart guide

1. Check the system requirments and compatible software
2. Install Intel RealSense Depth Camera Manager 
3. Install RSSDK
4. Install Visual Studio 2013
5. Install Qt 5
6. Install Qt 5 Visual Studio 2013 Add-in
7. Add RSSDK property sheet to project
8. Compile and run 

#Environment setup

## Intel RealSense

At the time, the Intel RealSense SDK (Intel RSSDK) was in it's first stable release R3 V5.0.3.7777

Follow the instruction on the [RSSDK download page][rssdk download]

1.  Install the "Intel RealSense Depth Camera Manager". F200 DCM V1.2 Build 1.3.20.55679 was used.
2.  Install the RSSDK. R3 V5.0.3.7777 (5.0.3.187777) was used.

Notes:

* Ensure the Intel RealSense camera is plugged in during installation.
* At the time of writing, the RSSDK had some minor reliablility issues. **Make sure the camera is plugged into a suitable port PRIOR to booting the machine**, the camera may also fail to work after the machine has gone to sleep or been hibernated. The solution is to restart the machine with the camera plugged in.

## Visual Studio 2013

1. Install a compatible version of Visual Studio.
2. Add the RSSDK property sheet to your project.
3. Integrate Qt

### 1. VS Version

According the the [SDK Developer Guide][SDK Developer Guide] the supported development environment is Microsoft Visual Studio 2010-2013 with the latest service pack.

For referece: VS 2013 was used and at the time of writing, VS 2015 could not compile a RealSense Application.

### 2. RSSDK integration (property sheets)

Simple mode: Add the included Property Sheets
From inside Visial studio with the project open

1. Open the property manager by View  Other Windows  Property Manager.
2. Right click on the project name and choose Add Existing Property Sheet.
3. Select `$(RSSDK_DIR)/props/VS201013.Integration.MD.props`

Note:

* $(RSSDK_DIR) by default in R3 is `C:\Program Files (x86)\Intel\RSSDK`
* See $(RSSDK_DIR)/props for more property sheets. This project utilises the sheet listed above.
* See the [SDK Developer Guide][SDK Developer Guide] section on configuring development environment for more information.

### 3. Qt Integration

#### Configure VS to point to `"bin/qmake"` and `"lib/libraries"`

In Visual studio:

1. go to QT5->QT Options
2. Click the Qt Versions tab
3. Click Add and add the path

On the development machine that was `C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl`. You're location may vary.

#### Set build options

* Check properties options in linker settings at: Properties > Configuration Properties > Linker > Advanced > Target Machine. In my case, it should be x64.
* Select Build > Configuration Manager from the main menu in visual studio. Make sure our project has the correct platform specified. 
* Right click the project > Qt Project Settings > Under the Properties tab > Version (select the version we specifiec above.

### Other
You may also find the Visual Studio Extension [Markdown Mode][markdown mode] (a language service for editing Markdown files) usefull.

## Qt

Go to the [Qt open source downloads page][qt opensource downloads] (click view all downloads if not visible).

1. Install Qt
2. Install the Qt Visual Studio Add-in

In that order

For reference, the development machine used:

* Qt 5.4.2 for Windows 64-bit (VS 2013)
* Visual Studio Add-in 1.2.4 for Qt5

Note: You may need to alter the project settings and properties to compile Qt interface in VS. Instructions in the Visual Studio section.


[rssdk download]: https://software.intel.com/en-us/intel-realsense-sdk/download
[qt opensource downloads]: http://www.qt.io/download-open-source/#section-2
[Markdown Mode]: https://visualstudiogallery.msdn.microsoft.com/0855e23e-4c4c-4c82-8b39-24ab5c5a7f79
[SDK Developer Guide]: http://a





 