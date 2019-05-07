/*
 * Copyright (c) 2018 Pilz GmbH & Co. KG
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <condition_variable>
#include <mutex>
#include <thread>
#include <atomic>

#include <gtest/gtest.h>

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>

#include <prbt_hardware_support/brake_test_utils.h>
#include <prbt_hardware_support/brake_test_utils_exception.h>

namespace brake_test_utils_test
{

using namespace prbt_hardware_support;

static constexpr unsigned int JOINT_STATES_TOPIC_QUEUE_SIZE {1};

class JointStatesPublisher
{
  public:
    JointStatesPublisher();
    void start_async();
    void terminate();
  private:
    void start();
  private:
    ros::NodeHandle nh_;
    std::thread publisher_thread_;
    std::atomic_bool terminate_;
};

JointStatesPublisher::JointStatesPublisher()
{
}

void JointStatesPublisher::start_async()
{
  terminate_ = false;
  publisher_thread_ = std::thread{ [this]{ this->start(); } };
}

void JointStatesPublisher::terminate()
{
  terminate_ = true;
  publisher_thread_.join();
}

void JointStatesPublisher::start()
{
  ros::Publisher joint_states_pub {nh_.advertise<sensor_msgs::JointState>(TOPIC_NAME, JOINT_STATES_TOPIC_QUEUE_SIZE)};
  sensor_msgs::JointState msg;
  msg.name = {"joint1", "joint2"};
  msg.position = {0.1, 0.11};
  while (!terminate_)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    joint_states_pub.publish(msg);
  }
}

/**
 * Test the getCurrentJointStates utility function
 *
 * Test Sequence:
 *  1. Call getCurrentJointStates() and publish on the joint_states topic asynchronously
 *  2. Call getCurrentJointStates() without publishing to the joint_states topic
 *
 * Expected Results:
 *  1. Published message is obtained
 *  2. Exception is thrown
 */
TEST(BrakeTestUtilsTest, testGetCurrentJointStates)
{
  /**********
   * Step 1 *
   **********/
  JointStatesPublisher joint_states_publisher;
  joint_states_publisher.start_async();

  try
  {
    BrakeTestUtils::getCurrentJointStates();
  }
  catch(const std::exception& e)
  {
    FAIL() << e.what() << '\n';
  }

  /**********
   * Step 2 *
   **********/
  joint_states_publisher.terminate();

  ASSERT_THROW(BrakeTestUtils::getCurrentJointStates(), GetCurrentJointStatesException);
}

/**
 * Test the compareJointStatePositions utility function
 *
 * Test Sequence:
 *  1. Compare two equal joint states
 *  2. Compare two joint states with a difference in all joints slightly below the tolerance
 *  3. Compare two joint_states with a difference in one joint slightly above the tolerance,
 *     repeat the test for all joints.
 *
 * Expected Results:
 *  1. Returns true
 *  2. Returns true
 *  3. Always returns false
 */
TEST(BrakeTestUtilsTest, testCompareJointStatePositions)
{
  SUCCEED();
}

/**
 * TODO
 */
TEST(BrakeTestUtilsTest, testDetectRobotMotion)
{
  SUCCEED();
}

} // namespace brake_test_utils_test

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "unittest_brake_test_utils");
  ros::NodeHandle nh;

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}