#include <ros/ros.h>
#include <dji_osdk_ros/common_type.h>
#include <geometry_msgs/Twist.h>
#include <dji_osdk_ros/FlightTaskControl.h>
#include <dji_osdk_ros/SetGoHomeAltitude.h>
#include <dji_osdk_ros/GetGoHomeAltitude.h>
#include <dji_osdk_ros/SetCurrentAircraftLocAsHomePoint.h>
#include <dji_osdk_ros/SetAvoidEnable.h>
#include <dji_osdk_ros/ObtainControlAuthority.h>
#include <dji_osdk_ros/EmergencyBrake.h>
#include <dji_osdk_ros/GetAvoidEnable.h>
#include <dji_osdk_ros/JoystickParams.h>
#include<dji_osdk_ros/SetJoystickMode.h>
#include<dji_osdk_ros/JoystickAction.h>
#include "codeencode.h"
#include "std_msgs/UInt8MultiArray.h"
#include <memory.h>

using namespace dji_osdk_ros;
Interface_Message stOMessage;
FlightTaskControl control_task;
ros::ServiceClient task_control_client;
ros::ServiceClient set_joystick_mode_client;
ros::ServiceClient joystick_action_client;
int takeoff_flag = 1;
JoystickCommand vel_cmd;
//位置控制
bool moveByPosOffset(FlightTaskControl& task,const JoystickCommand &offsetDesired,
                     float posThresholdInM = 0.8,
                     float yawThresholdInDeg = 1.0);
//速度控制
void velocityAndYawRateCtrl(const JoystickCommand &offsetDesired, uint32_t timeMs);

std_msgs::UInt8MultiArray cmsg[];

void domsg(const std_msgs::UInt8MultiArray::ConstPtr& msg)
{
  cmsg->data=msg->data;
  //stOMessage为地面站输入指令
  fnCluster_codec_DecodeInterfaceMsg(&cmsg->data[0], &stOMessage, msg->data.size());
  //先判断输入的命令类型
  //起飞 cmd=1且降落标志等于1
  if (stOMessage.enUavCmd==1 && takeoff_flag==1)
  {
    control_task.request.task = FlightTaskControl::Request::TASK_TAKEOFF;
    ROS_INFO_STREAM("Takeoff request sending ...");
    task_control_client.call(control_task);
    if(control_task.response.result == false)
    {
      ROS_ERROR_STREAM("Takeoff task failed");
    }
    if(control_task.response.result == true)
    {
      ROS_INFO_STREAM("Takeoff task successful");
      takeoff_flag=0;
    }  
  }
  //降落 
  if (stOMessage.enUavCmd==2 && takeoff_flag==0)
  {
    control_task.request.task = FlightTaskControl::Request::TASK_LAND;
    ROS_INFO_STREAM("Land request sending ...");
    if(control_task.response.result == true)
    {
      ROS_INFO_STREAM("Land task successful");
    }
    else
    {
      ROS_INFO_STREAM("Land task failed.");
    }
  }
  //速度控制
  if (stOMessage.enUavCmd==6 && takeoff_flag==0)
  {
    vel_cmd.x=stOMessage.stVelocity.stPoint.x;
    vel_cmd.y=stOMessage.stVelocity.stPoint.y;
    vel_cmd.z=stOMessage.stVelocity.stPoint.z;
    vel_cmd.yaw=0;
    //没有传输过来的时间 暂时预设为1s 即沿输入的速度飞行一秒
    velocityAndYawRateCtrl(vel_cmd, 1000); 
  }   
};
int main(int argc, char** argv)
{
    
  ros::init(argc, argv, "Mult_Single_node");
  ros::NodeHandle nh;
  task_control_client = nh.serviceClient<FlightTaskControl>("/flight_task_control");
  auto set_go_home_altitude_client = nh.serviceClient<SetGoHomeAltitude>("/set_go_home_altitude");
  auto get_go_home_altitude_client = nh.serviceClient<GetGoHomeAltitude>("get_go_home_altitude");
  auto set_current_point_as_home_client = nh.serviceClient<SetCurrentAircraftLocAsHomePoint>("/set_current_aircraft_point_as_home");
  auto enable_horizon_avoid_client  = nh.serviceClient<SetAvoidEnable>("/set_horizon_avoid_enable");
  auto enable_upward_avoid_client   = nh.serviceClient<SetAvoidEnable>("/set_upwards_avoid_enable");
  auto get_avoid_enable_client      = nh.serviceClient<GetAvoidEnable>("get_avoid_enable_status");
  auto obtain_ctrl_authority_client = nh.serviceClient<dji_osdk_ros::ObtainControlAuthority>("obtain_release_control_authority");
  auto emergency_brake_client       = nh.serviceClient<dji_osdk_ros::EmergencyBrake>("emergency_brake");
  set_joystick_mode_client = nh.serviceClient<SetJoystickMode>("set_joystick_mode");
  joystick_action_client   = nh.serviceClient<JoystickAction>("joystick_action");
  EmergencyBrake emergency_brake;
  //初始化飞行任务
  ObtainControlAuthority obtainCtrlAuthority;
  //获取控制权限
  obtainCtrlAuthority.request.enable_obtain = true;
  obtain_ctrl_authority_client.call(obtainCtrlAuthority);
  //话题 cmd_mult 用来接收码流 收取信息
  ros::Subscriber sub = nh.subscribe<std_msgs::UInt8MultiArray>("cmd_mult",1000,domsg);
  //话题state更新飞机状态并传递信息
  ros::Publisher pub=nh.advertise<std_msgs::UInt8MultiArray>("state",1000);
  ros::Rate loop_rate(ROS_LOOP_FREQUENCY);

}