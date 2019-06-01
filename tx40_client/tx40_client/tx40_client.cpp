// client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <cpprest/http_client.h>
#include <cpprest/json.h>
//#pragma comment(lib, "cpprest_2_10")

using namespace web;
using namespace web::http;
using namespace web::http::client;

#include <iostream>
using namespace std;

void display_json(
	json::value const & jvalue,
	utility::string_t const & prefix)
{
	wcout << prefix << jvalue.serialize() << endl;
}

pplx::task<http_response> make_task_request(
	http_client & client,
	method mtd,
	utility::string_t const & task,
	json::value const & jvalue)
{
	return (mtd == methods::GET || mtd == methods::HEAD) ?
		client.request(mtd, task) :
		client.request(mtd, task, jvalue);
}

void make_request(
	http_client & client,
	method mtd,
	utility::string_t const & task,
	json::value const & jvalue)
{
	make_task_request(client, mtd, task, jvalue)
		.then([](http_response response)
	{
		if (response.status_code() == status_codes::OK)
		{
			wcout << "OK ";
			return response.extract_json();
		}
		wcout << "FAIL ";
		return pplx::task_from_result(json::value());
	})
		.then([](pplx::task<json::value> previousTask)
	{
		try
		{
			display_json(previousTask.get(), L"R: ");
		}
		catch (http_exception const & e)
		{
			wcout << e.what() << endl;
		}
	})
		.wait();
}

int main()
{
	std::cout << "Hello World! \nThis is client.";

	http_client client(U("http://localhost:5000/v1/tx40"));
	utility::string_t task = L"state";

	//* Get some specific parameters
	//auto getvalue = json::value::array();
	//getvalue[0] = json::value::string(L"one");
	//getvalue[1] = json::value::string(L"two");
	//getvalue[2] = json::value::string(L"three");

	//wcout << L"\nPOST (get some values)\n";
	//display_json(getvalue, L"S: ");
	//make_request(client, methods::POST, task, getvalue);

	////* Delete a specific parameter
	//auto delvalue = json::value::array();
	//delvalue[0] = json::value::string(L"one");

	//wcout << L"\nDELETE (delete values)\n";
	//display_json(delvalue, L"S: ");
	//make_request(client, methods::DEL, task, delvalue);

	//wcout << L"\nPOST (get some values)\n";
	//display_json(getvalue, L"S: ");
	//make_request(client, methods::POST, task, getvalue);

	//* Get robot state
	auto nullvalue = json::value::null();

	wcout << L"\nGET (get state)\n";
	display_json(nullvalue, L"S: ");
	make_request(client, methods::GET, L"state", nullvalue);

	//* Put robot state
	auto putstate = json::value::boolean(true); // create parameters for request

	wcout << L"\nPUT (change state)\n";
	display_json(putstate, L"S: "); // display request
	make_request(client, methods::PUT, L"state", putstate); // send request to server

	//* Get robot state
	wcout << L"\nGET (get state)\n";
	display_json(nullvalue, L"S: ");
	make_request(client, methods::GET, L"state", nullvalue);

	//* Get robot position
	wcout << L"\nGET (get position)\n";
	display_json(nullvalue, L"S: ");
	make_request(client, methods::GET, L"position", nullvalue);

	//* Put robot position
	auto putvalue = json::value::object(); // create parameters for request
	putvalue[L"x"] = 100;
	putvalue[L"y"] = 200;
	putvalue[L"z"] = 300;
	putvalue[L"rx"] = 20;
	putvalue[L"ry"] = 30;
	putvalue[L"rz"] = 40;
	putvalue[L"linhtinh"] = 50;

	wcout << L"\nPUT (add values)\n";
	display_json(putvalue, L"S: "); // display request
	make_request(client, methods::PUT, L"position", putvalue); // send request to server

	//* Get robot position
	wcout << L"\nGET (get position)\n";
	display_json(nullvalue, L"S: ");
	make_request(client, methods::GET, L"position", nullvalue);

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
