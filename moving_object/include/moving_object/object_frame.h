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
#ifndef ICA_MOVING_OBJECT_FRAME_H
#define ICA_MOVING_OBJECT_FRAME_H

#include <ros/ros.h>
#include <vector>
#include <dynamic_reconfigure/server.h>
#include "moving_object/consts.h"

namespace intelligent_ca
{
/** @brief This class merges topics from object pipeline component, and publish
 * the moving topics.
 * This class stores and manages the objects tracked in one camera frame.
 */
class MovingObjectFrame
{
public:
  MovingObjectFrame();
  MovingObjectFrame(const ros::NodeHandle nh);
  MovingObjectFrame(const ros::Time& stamp, const std::string& frame_id,
                    const ros::NodeHandle nh = ros::NodeHandle("~"));
  virtual ~MovingObjectFrame();

  // init(ObjectVector& vector);

  /** @brief add detection vector (got from object pipeline) to the object frame.
   *  @param[in] vector detection vector storing detection info calculated by object module.
   */
  void addVector(const DetectionVector& vector);

  /** @brief add tracking vector (got from object pipeline) to the object frame.
   *  @param[in] vector Tracking vector storing detection info calculated by object module.
   */
  void addVector(const TrackingVector& vector);

  /** @brief add Localization vector (got from object pipeline) to the object frame.
   *  @param[in] vector Localication vector storing detection info calculated by object module.
   */
  void addVector(const LocalizationVector& vector);

  /** @brief Publish messages with moving object topics.
   *  @return true if correctly published;
   *          false otherwise.
   */
  bool publish();

  /** @brief Check if object information is enough for further calculating.
   *         Object information is the one got from object module, such as Detection Vector, Tracking Vector and
   *         Localization Vector.
   *         The function is normally invoked before generate or publish the moving topics.
   *         @return true if data ready, otherwise false.
   */
  bool isDataReady()
  {
    return !objects_detected_.empty() && !objects_tracked_.empty() && !objects_localized_.empty();
  };

  /** @brief Get transform frame id, which is used for message generation.
   *         The class stores the frame id getting from object topics.
   *  @return the string with content of transform frame id.
   */
  std::string getTfFrameId()
  {
    return tf_frame_id_;
  };

  /** @brief  Get time stamp, which is used for message generation.
   *          The class stores the timestamp getting from object topics.
   *  @return The ros::Time value of message.
   */
  ros::Time getStamp()
  {
    return stamp_;
  };

  /** @brief Merge separate info of detection, tracking and localization into one topic.
   */
  void mergeObjects();

  /** @brief find the moving object which has the same ROI as the param roi.
   *  @param[in] roi Region of interest within an image, which is used to identify the different objects in an image.
   *  @param[out]track The moving object if found.
   *  @return true if found, otherwise false.
   */
  bool findMovingObjectByRoi(const ObjectRoi& roi, MovingObject& track);
  bool findMovingObjectById(const int id, MovingObject& out);

  MovingObjectVector& getMovingObjects()
  {
    return moving_objects_;
  };

  /** @brief Get the centroid of an object [Static Function]
   *  @param[in] ob The object to be calculated.
   *  @return The centroid point of the given object.
   */
  static geometry_msgs::Point32 getCentroid(MovingObject& ob)
  {
    geometry_msgs::Point32 out;
    out.x = (ob.min.x + ob.max.x) / 2;
    out.y = (ob.min.y + ob.max.y) / 2;
    out.z = (ob.min.z + ob.max.z) / 2;
    return out;
  }

  /** @brief Check if a moving object is social object.
   *  As current design, social object list:
   *  1. People
   *  2. Robot base
   *  @param[in] ob The moving object (from object pipeline) to be checked.
   *  @return true if the given object is in social type, otherwise false.
   */
  bool isSocialObject(MovingObject& ob);

private:
  // void publishObjectsInGroup();

  /** @brief find the tracking object from the tracking vector which has the same ROI.
   *  @param[in] roi Region of interest within an image, which is used to identify the different objects in an image.
   *  @param[out]track The Tracking object if found.
   *  @return true if found, otherwise false.
   */
  bool findTrackingObjectByRoi(const ObjectRoi& roi, TrackingObjectInBox& track);

  /** @brief find the localization object from the tracking vector which has the same ROI.
   *  @param[in] roi Region of interest within an image, which is used to identify the different objects in an image.
   *  @param[out]track The Tracking object if found.
   *  @return true if found, otherwise false.
   */
  bool findLocalizationObjectByRoi(const ObjectRoi& roi, LocalizationObjectInBox& loc);

  /** @brief Set flag for publish status in case to publish twice.
   *  @param[in] state Publish state to be set in the class.
   */
  void setFlagPublished(bool state);

  void initParameter(void);

  /**< @brief transform frame id which is archived from object topics and is generated for moving topics.*/
  std::string tf_frame_id_;

  /**< @brief time stamp which is archived from object topics and is generated for moving topics.*/
  ros::Time stamp_;

  ros::NodeHandle nh_;

  /**< @brief vectors storing object info(detection, tracking and localization, got from object topics).*/
  DetectionVector objects_detected_;
  TrackingVector objects_tracked_;
  LocalizationVector objects_localized_;

  ros::Publisher moving_objects_pub_; /**<@brief ros publisher for moving object topic.*/
  ros::Publisher social_object_pub_;  /**<@brief ros publisher for social object topic.*/
  MovingObjectVector moving_objects_; /**<@brief vector storing the moving objects.*/
  bool published_;                    /**<@brief published status for the moving topic vector. true means published.*/

  bool social_msg_enabled_;
  bool moving_object_msg_enabled_;
  double posibility_threshold_;

  bool is_merging_;

  std::vector<std::string> social_filter_;  // filter strings for social messages
};

}  // namespace
#endif
