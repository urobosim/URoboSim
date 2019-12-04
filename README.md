# URoboSim

Unreal engine based robot simulator.

Version using `SDF` for loading robots.
See `gzsdf` for converting `URDF` to `SDF`.

**Dependencies**

*  [UUtils](https://github.com/urobosim/UUtils)
*  [UROSWorldControl](https://github.com/urobosim/UROSWorldControl)
*  [UROSBridge](https://github.com/urobosim/UROSBridge)
*  [URoboVision](https://github.com/urobosim/URoboVision)

**Installation**

*  place the URobosim into the plugin folder of your project
*  place all dependencies into the plugin folder of your project
*  add URoboSim as dependency to the *.Build.cs

**Creating the robot model**

*  import the sdf file (enable: Combine Meshes)
*  drag and dorp the sdf file into the map (while dragging wait for the mesh creation to be finished before dropping the model)

**Add Controller**

*  add RControllerComponent to the model
*  add new element to the ControllerList (Left field: Controller Name, Right field: Controller Type)
*  BaseController:
    *  Base Name: Name of the Base
*  JointController:
    *  Base Link: same as BaseController Base Name
    *  IgnoreList: Joints that are not controlled
*  CameraController:
    *  CameraRef: Name of the Reference Link
    *  Camera Name: Name of the Camera (Placed in the world)
    *  Perceived Object: List of Perceived object, used for fake perception
    *  PoseOffset: offset of the camera from ReferenceFrame
*  HeadController
*  GripperController(only tested on PR2):
    *  GripperCompSetting: Setting of the Gripper Component (ReferenceFrame, Offset, Size of graspeble objects)
    *  Stiffness
    *  Max Force
    *  Dampening
    *  Right/LeftJointName:
    *  Right/LeftFingerTipName
    *  GraspComponent: Name of the GraspComponent (GraspComponent has to be added to model)