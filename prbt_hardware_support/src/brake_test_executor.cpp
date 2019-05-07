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

#include <prbt_hardware_support/brake_test_executor.h>

#include <prbt_hardware_support/brake_test_utils.h>


namespace prbt_hardware_support
{

BrakeTestExecutor::BrakeTestExecutor(ros::NodeHandle& nh)
  :nh_(nh)
{
  brake_test_srv_ = nh_.advertiseService("prbt/execute_braketest",
                                         &BrakeTestExecutor::executeBraketest,
                                         this);
}


bool BrakeTestExecutor::executeBraketest(BrakeTest::Request&,
                                         BrakeTest::Response& response)
{
  if (BrakeTestUtils::detectRobotMotion())
  {
    response.msg = "Robot is moving, cannot perform brake test";
    response.result = BrakeTest::Response::ROBOT_MOTION_DETECTED;
    return true;
  }

  // TODO: Get brake test duration

  // FOR EACH JOINT:
    // TODO: Trigger brake test via CAN
    // TODO: Check result of brake test
  // END-FOR EACH

  response.result = BrakeTest::Response::SUCCESS;
  return true;
}

}
