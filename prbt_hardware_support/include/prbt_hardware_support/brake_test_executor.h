/*
 * Copyright (c) 2019 Pilz GmbH & Co. KG
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

#ifndef BRAKE_TEST_EXECUTOR_H
#define BRAKE_TEST_EXECUTOR_H

#include <ros/ros.h>

#include <prbt_hardware_support/BrakeTest.h>

namespace prbt_hardware_support
{

/**
 * @brief Executes the brake test for all joints. A brake test is requested
 * via service call.
 *
 */
class BrakeTestExecutor
{
public:
  BrakeTestExecutor(ros::NodeHandle& nh);

protected:
  bool executeBraketest(BrakeTest::Request&, BrakeTest::Response& response);

private:
  ros::NodeHandle nh_;
  //! Service which can be called by the user to trigger and execute a
  //! brake test for all joints.
  ros::ServiceServer brake_test_srv_;

};

} // namespace prbt_hardware_support
#endif // BRAKE_TEST_EXECUTOR_H
