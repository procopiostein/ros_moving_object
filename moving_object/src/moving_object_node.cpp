
/*
 * Copyright (c) 2017 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <ros/ros.h>
#include "moving_object/moving_object_ros.h"

using namespace intelligent_ca;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "moving_object");
  ros::NodeHandle node_handler("~");

  ROS_INFO("ENTER moving_object ROS node. ");

  MovingObjectRos merger(node_handler);

  ros::spin();

  return (0);
}
