// tx40_service.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <map>
#include <set>
#include <string>

#include <boost/format.hpp>
#include <boost/thread/thread.hpp>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/base_uri.h>

#include "utils.h"
#include "soapH.h"
#include "soapStub.h"
#include "stdsoap2.h"
#include "soapCS8ServerV0Proxy.h"
#include "soapCS8ServerV1Proxy.h"
#include "soapCS8ServerV3Proxy.h"
#include "TX60L.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

using namespace std;

#define VIRTUAL_IP_ADDRESS "http://127.0.0.1:5653/"
#define REAL_IP_ADDRESS1 "http://172.31.0.3:5653/"      // config ip address: 172.31.0.10; subnet mask: 255.255.0.0
#define REAL_IP_ADDRESS2 "http://192.168.0.252:5653/"   // config ip address: 192.168.0.254; subnet mask: 255.255.0.0
//#define ROBOT_IP VIRTUAL_IP_ADDRESS
#define ROBOT_IP REAL_IP_ADDRESS2
#define TIMEOUT 100
#define SERVICE_URL L"http://localhost:5000/v1/tx40/"

#define TRACE(msg)            wcout << msg
#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"

map<utility::string_t, utility::string_t> dictionary;
TX60L * mRobot;
bool mRobotEnabled = false;
double mX = 0, mY = 0, mZ = 0, mRX = 0, mRY = 0, mRZ = 0;

void sleep(int ms)
{
	boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}

bool isEnabled()
{
	return mRobotEnabled;
}

/*  Enable the robot
		:param robot	: object, robot
		:return: int, ...
	Usage:
		int ret = enable(robot);
*/
int enable(TX60L * robot)
{
	int ret;
	if (!mRobotEnabled)
	{
		ret = robot->Login(ROBOT_IP, "default", "");
		wcout << ret << endl;
		if (ret > 0)
		{
			sleep(1000);
			robot->ResetMotion();
			robot->Power(true);
			mRobotEnabled = true;
		}
	}
	else ret = -1;
	wcout << ret << endl;
	return ret;
}

/*  Disable the robot
		:param robot	: object, robot
		:return: int, ...
	Usage:
		bool ret = enable(robot);
*/
int disable(TX60L * robot)
{
	if (!isEnabled()) return -1;

	robot->Power(false);
	robot->Logoff();
	mRobotEnabled = false;

	return 1;
}

/*  Retrieve the Cartesian position of the robot
		:param robot                : object, robot
		:param pos					: vector<double>, vector of 6DOF position
		:return: bool, True: it is able to retrieve, False: retrieving is invalid
	Usage:
		bool status = getPos(robot, position);
*/
bool getPos(TX60L * robot, std::vector<double> * pos)
{
	if (!isEnabled()) return false;

	robot->GetRobotCartesianPosition((*pos));

	return true;
}

/*  Show current position in units (milimeters, radians)
		:param      pos         : vector<double>, position with (x, y, z) is m unit and (rx, ry, rz) is radian unit
		:return:    mmdeg_pos   : vector<double>, position with (x, y, z) is mm unit and (rx, ry, rz) is degree unit
	Usage:
		showPoint(pos);
*/
void showPos(std::vector<double> pos)
{
	std::cout << "\nCurrent cartesian position is: ";
	showVect(mrad2mmdeg(pos));
}

/*  Check whether robot arrived the position vector
		:param robot    : object, robot
		:param pos  : vector<double>, terminated position vector of 6DOF position
		:return: bool, True: already arrived, False: still move
	Usage:
		bool arrived = isArrivedVect(robot, pos);
*/
bool isArrivedVect(TX60L * robot, std::vector<double> pos)
{
	std::vector<double> cur_pos;
	getPos(robot, &cur_pos);
	return IsVectEqual(cur_pos, pos);
}

/**
Move robot to a position of meter-radian vector

Args:
	robot (object)			: robot
	pos (vector<double>)	: position coords-directions in meter-radian units

Returns:
	bool: True if moving is successful, False if moving is timeout

Usage:
	bool status = movePoint(robot, mrad_pos);
*/
bool movePos(TX60L * robot, std::vector<double> pos)
{
	if (!isEnabled()) return false;

	/* Standardize input values to vector */
	// firstly adjust angle into the range [-180, 180]
	// secondly, assign (x, y, z, rx, ry, rz) into a vector
	// finally, convert vector of mm-degree to m-radian
	//std::vector<double> req_pos = mmdeg2mrad(pos); // requested position

	/* Execute */
	robot->Move(pos);

	/**================================================== *
	 * ==========  Wait till finish moving or timeout  ========== *
	 * ================================================== */
	robot->ResetStopCounter();
	while (1) {
		if (isArrivedVect(robot, pos))
		{
			std::cout << "\nReached termination.";
			break; // stop waiting if robot already arrived the termination
		}
		else if (robot->CountStopTimes() > TIMEOUT) // stop waiting if timeout
		{
			std::cout << "\nStop before absolutely reaching termination.";
			break;
		}
	}
	/* =======  End of Wait till finish moving or timeout  ======= */

	return true;
}

/*  Display in-out message
		:param jvalue	: json::value, data of message
		:param prefix	: utility::string_t, prefix of message such as: S(send) or R(receive)
	Usage:
		display_json(answer, L"S: ");
*/
void display_json(
	json::value const & jvalue,
	utility::string_t const & prefix)
{
	wcout << prefix << jvalue.serialize() << endl;
}


void handle_get(http_request request)
{
	TRACE(L"\nhandle GET\n");

	auto paths = http::uri::split_path(http::uri::decode(request.relative_uri().path()));

	switch (paths.size())
	{
	case 1:
		if (paths[0] == L"state")
		{
			auto answer = json::value::object();

			answer = json::value::boolean((mRobotEnabled > 0)); // make response

			display_json(json::value::null(), L"R: "); // display received request
			display_json(answer, L"OK S: "); // display response

			request.reply(status_codes::OK, answer); // send response and show status code
		}
		else if (paths[0] == L"position")
		{
			// retrieve current position(in Cartesian units) of the robot
			std::vector<double> cur_pos;
			getPos(mRobot, &cur_pos);
			cur_pos = mrad2mmdeg(cur_pos); // convert meter-radian units to milimeter-degree units

			// prepare response data of message
			auto answer = json::value::object();
			answer[L"x"] = cur_pos[0];
			answer[L"y"] = cur_pos[1];
			answer[L"z"] = cur_pos[2];
			answer[L"rx"] = cur_pos[3];
			answer[L"ry"] = cur_pos[4];
			answer[L"rz"] = cur_pos[5];

			display_json(json::value::null(), L"R: "); // display received request
			display_json(answer, L"OK S: "); // display response

			request.reply(status_codes::OK, answer); // send response and show status code
		}
		else
		{
			wcout << "FAIL" << endl;
			request.reply(status_codes::NotFound);
		}
		break;

	default:
		wcout << "FAIL" << endl;
		request.reply(status_codes::NotFound);
		break;
	}
}

void handle_request(
	http_request request,
	function<void(json::value const &, json::value &)> action)
{
	auto answer = json::value::object();

	request
		.extract_json()
		.then([&answer, &action](pplx::task<json::value> task) {
		try
		{
			auto const & jvalue = task.get();
			display_json(jvalue, L"R: ");

			if (!jvalue.is_null())
			{
				action(jvalue, answer);
			}
		}
		catch (http_exception const & e)
		{
			wcout << e.what() << endl;
		}
	})
		.wait();


	display_json(answer, L"S: ");

	request.reply(status_codes::OK, answer);
}

void handle_post(http_request request)
{
	TRACE("\nhandle POST\n");

	auto paths = http::uri::split_path(http::uri::decode(request.relative_uri().path()));

	switch (paths.size())
	{
	case 1:
		if (paths[0] == L"state")
		{
			request.reply(status_codes::NotImplemented);
		}
		else if (paths[0] == L"position")
		{
			request.reply(status_codes::NotImplemented);
		}
		else
		{
			request.reply(status_codes::NotFound);
		}
		break;

	default:
		request.reply(status_codes::NotFound);
		break;
	}
}

void handle_put(http_request request)
{
	TRACE("\nhandle PUT\n");

	auto paths = http::uri::split_path(http::uri::decode(request.relative_uri().path()));

	switch (paths.size())
	{
	case 1:
		if (paths[0] == L"state")
		{
			handle_request(
				request,
				[](json::value const & jvalue, json::value & answer)
			{
				//* Retrieve request
				bool request_state = jvalue.as_bool();
				if (request_state)
				{
					if (enable(mRobot))
					{
						mRobotEnabled = true;

						TRACE_ACTION(L"enabled", L"mRobotEnabled", mRobotEnabled);
						answer[L"state"] = json::value::string(L"<updated>");
					}
					else
					{
						mRobotEnabled = false;

						TRACE_ACTION(L"not enabled", L"mRobotEnabled", mRobotEnabled);
						answer[L"state"] = json::value::string(L"<not updated>");
					}
				}
				else
				{
					if (disable(mRobot))
					{
						TRACE_ACTION(L"disabled", L"mRobotEnabled", mRobotEnabled);
						answer[L"state"] = json::value::string(L"<updated>");
					}
					else
					{
						TRACE_ACTION(L"already disabled", L"mRobotEnabled", mRobotEnabled);
						answer[L"state"] = json::value::string(L"<not updated>");
					}
				}
			});
		}
		else if (paths[0] == L"position")
		{
			//request.reply(status_codes::NotImplemented);
			handle_request(
				request,
				[](json::value const & jvalue, json::value & answer)
			{
				auto jobject = jvalue.as_object();
				auto x = jobject.find(L"x");
				auto y = jobject.find(L"y");
				auto z = jobject.find(L"z");
				auto rx = jobject.find(L"rx");
				auto ry = jobject.find(L"ry");
				auto rz = jobject.find(L"rz");

				bool condition = (x != jobject.end()) && (y != jobject.end()) && (z != jobject.end()) 
					&& (rx != jobject.end()) && (ry != jobject.end()) && (rz != jobject.end());

				if (condition)
				{
					std::vector<double> req_pos = value2vector((*x).second.as_double(), (*y).second.as_double(), (*z).second.as_double(), 
						(*rx).second.as_double(), (*ry).second.as_double(), (*rz).second.as_double());
					req_pos = mmdeg2mrad(req_pos);

					bool status = movePos(mRobot, req_pos);
					if (status)
					{
						TRACE_ACTION(L"executed", L"moving", L"successfully");
						answer[L"moving"] = json::value::string(L"<successfully>");
					}
					else
					{
						TRACE_ACTION(L"executed", L"moving", L"unsuccessfully");
						answer[L"moving"] = json::value::string(L"<unsuccessfully>");
					}					
				}
			});
		}
		else
		{
			request.reply(status_codes::NotFound);
		}
		break;

	default:
		request.reply(status_codes::NotFound);
		break;
	}
}

void handle_del(http_request request)
{
	TRACE("\nhandle DEL\n");

	auto paths = http::uri::split_path(http::uri::decode(request.relative_uri().path()));

	switch (paths.size())
	{
	case 1:
		if (paths[0] == L"restdemo")
		{
			request.reply(status_codes::NotImplemented);
		}
		else
		{
			request.reply(status_codes::NotFound);
		}
		break;

	default:
		request.reply(status_codes::NotFound);
		break;
	}
}

int main()
{
	std::cout << "Hello World! \nThis is server.";

	/**================================================== *
	 * ==========  Robot  ========== *
	 * ================================================== */
	/*--------  Initialize robot  --------*/
	mRobot = new TX60L;
	
	/*--------  Enable robot  --------*/
	int ret = enable(mRobot);
	if (ret == 0) return -1;

	std::vector<double> position;

	// Test reading position
	getPos(mRobot, &position);
	showPos(position);

	// Test moving to a position
	movePos(mRobot, mmdeg2mrad(value2vector(-260, 315, 50, 180, 0, 0))); // move to neutral safe position
	getPos(mRobot, &position);
	showPos(position);

	// Test moving to a position
	movePos(mRobot, mmdeg2mrad(value2vector(-260, -75, 50, 180, 0, 0))); // move to initial position
	getPos(mRobot, &position);
	showPos(position);

	// Disable robot control
	disable(mRobot);

	/**================================================== *
	 * ==========  Service  ========== *
	 * ================================================== */
	http_listener listener(SERVICE_URL);

	listener.support(methods::GET, handle_get);
	listener.support(methods::POST, handle_post);
	listener.support(methods::PUT, handle_put);
	listener.support(methods::DEL, handle_del);

	try
	{
		listener
			.open()
			.then([&listener]() {TRACE(L"\nstarting to listen\n"); })
			.wait();

		while (true);
	}
	catch (exception const & e)
	{
		wcout << e.what() << endl;
	}

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
