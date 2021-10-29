#include <string>
#include <vector>
#include <set>

#include <Wt/Json/Object>
#include <Wt/Json/Array>
#include <Wt/Json/Value>
#include <Wt/Json/Parser>
#include <Wt/WException>

#include "BridgeController.h"
#include "BridgeConnector.h"
#include "Bridge.h"

using namespace std;

BridgeController::BridgeController() {}

BridgeController::~BridgeController() {}



void BridgeController::createBridgeUser(Bridge &bridge){
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api";
	string body = "{\"devicetype\":\"OurApplication#" + bridge.getUserName() + "\"}";

	string output = "";
	cout <<"***USING URL :" << url << endl;
	cout <<"***USING BODY :" << body << endl;
	
	BridgeConnector *http = new BridgeConnector();
	http->postRequest(url,body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
}


void BridgeController::getAllLights(Bridge bridge) {

	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/lights";	
	std::cout<<"----"+url+"======="<<std::endl;

	
	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
}

void BridgeController::getLightName(Bridge bridge, int lightID) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/lights/" + to_string(lightID);
	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));

}

void BridgeController::getLightStates(Bridge bridge, int lightID, int state) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/lights/" + to_string(lightID);

	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
}

void BridgeController::setLightName(Bridge bridge, int lightID, string newName) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/lights/" + to_string(lightID);
	
	string body = "{\"name\":\"" + newName + "\"}";
	
	string output = "";

	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url,body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
}

void BridgeController::setLightStateOn(Bridge bridge, int lightID, bool isOn) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/lights/" + to_string(lightID) + "/state";
	
	string body = "";
	
	if (isOn) {
		body = "{\"on\":true}";
	} else {
		body = "{\"on\":false}";
	}

	string output = "";
	
	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url,body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
}

void BridgeController::setLightStateHue(Bridge bridge, int lightID, int newHue) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/lights/" + to_string(lightID) + "/state";
	
	string body = "{\"hue\":" + to_string(newHue) + "}";
	
	string output = "";
	
	// call 'put' request from BridgeConnector
	
	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url,body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));

}

void BridgeController::setLightStateSat(Bridge bridge, int lightID, int newSat) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/lights/" + to_string(lightID) + "/state";
	
	string body = "{\"sat\":" + to_string(newSat) + "}";
	
	string output = "";
	
	// call 'put' request from BridgeConnector
	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url,body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	//return "";
}

void BridgeController::setLightStateBright(Bridge bridge, int lightID, int newBright) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/lights/" + to_string(lightID) + "/state";
	
	string body = "{\"bri\":" + to_string(newBright) + "}";
	
	string output = "";
	
	// call 'put' request from BridgeConnector
	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url,body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	//return "";
}

void BridgeController::setLightStateEffect(Bridge bridge, int lightID, int newEffect) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/lights/" + to_string(lightID) + "/state";
	
	string body = "";
	
	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url,body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
	// if(newEffect == 1) {
	// 	body = "{\"bri\":\"colorloop\"}";	
	// } else {
	// 	body = "{\"bri\":\"none\"}";
	// }

	// string output = "";
	
	// // call 'put' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.putRequest(url, body);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// //return "";
}

void BridgeController::getAllGroups(Bridge bridge) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups";

	string output = "";
	
	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
	// // call 'get' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.getRequest(url);
	
	// if (errorCode(output)) {
	// 	vector<string> outErr;
		
	// 	outErr.push_back(errorDescription(errorCode(output)));
	// 	outErr.push_back("0");
		
	// 	return outErr;
	// }
	
	// Wt::Json::Object jsonObj;
	// Wt::Json::parse(output, jsonObj);
	
	// set<string> idSet = jsonObj.names();
	// vector<string> idVec(idSet.begin(), idSet.end());
	
	// idVec.push_back("1");
	
	// //return idVec;
}

void BridgeController::getGroupName(Bridge bridge, int groupID) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID);
	
	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
	// string output = "";
	
	// // call 'get' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.getRequest(url);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// Wt::Json::Object jsonObj;
	// Wt::Json::parse(output, jsonObj);
	
	// string name = jsonObj.get("name");
	
	// return "Name: " + name;
}

void BridgeController::getGroupLights(Bridge bridge, int groupID) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID);
	
	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	

	// string output ="";
	
	// // call 'get' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.getRequest(url);
	
	// if (errorCode(output)) {
	// 	vector<string> outErr;
		
	// 	outErr.push_back(errorDescription(errorCode(output)));
	// 	outErr.push_back("0");
		
	// 	return outErr;
	// }
	
	// Wt::Json::Object jsonObj;
	// Wt::Json::parse(output, jsonObj);
	
	// Wt::Json::Array lightArr = jsonObj.get("lights");
	
	// lightArr.push_back("1");
	
	// vector<string> lightVec(lightArr.begin(), lightArr.end());
	
	// return lightVec;
}

void BridgeController::getGroupAction(Bridge bridge, int groupID, int action) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID);
	
	
	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
	
	// string output ="";
	
	// // call 'get' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.getRequest(url);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// Wt::Json::Object jsonObj;
	// Wt::Json::parse(output, jsonObj);
	
	// Wt::Json::Object actionObj = jsonObj.get("action");
	
	// int val;
	
	// switch (action) {
	// 	case 0:
	// 		if (actionObj.get("on")) {
	// 			return "On: True";
	// 		} else {
	// 			return "On: False";
	// 		}
	// 	case 1:
	// 		val = actionObj.get("hue");
	// 		return "Hue: " + to_string(val);
	// 	case 2:
	// 		val = actionObj.get("sat");
	// 		return "Saturation: " + to_string(val);
	// 	case 3:
	// 		val = actionObj.get("bri");
	// 		return "Brightness: " + to_string(val);
	// 	case 4: {
	// 		string str = actionObj.get("effect");
	// 		return "Effect: " + str;
	// 	}
	// 	default:
	// 		return "No action chosen.";
	// }
}

void BridgeController::setGroupName(Bridge bridge, int groupID, string newName) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID);
	
	string body = "{\"name\":\"" + newName + "\"}";

	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url, body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
	// string output = "";
	
	// // call 'put' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.putRequest(url, body);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// return "";
}

void BridgeController::setGroupLights(Bridge bridge, int groupID, std::vector<int> lights) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID);
	
	string body = "{\"lights\":[";
	
	for (unsigned int i = 0; i < lights.size(); i ++) {
		body += "\"" + to_string(lights[i])  + "\"";
		if(i < (lights.size() - 1))
			body += ",";
	}
	
	body += "]}";

	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url, body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
	// string output = "";
	
	// // call 'put' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.putRequest(url, body);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// return "";
}

void BridgeController::setGroupActionOn(Bridge bridge, int groupID, bool isOn) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID) +"/action";
	
	string body = "";
	
	if (isOn) {
		body ="{\"on\":true}";
	} else {
		body ="{\"on\":false}";
	} 
	
	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url, body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));

	// string output = "";
	
	// // call 'put' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.putRequest(url, body);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// return "";
}

void BridgeController::setGroupActionHue(Bridge bridge, int groupID, int newHue) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID) + "/action";
	
	string body = "{\"hue\":" + to_string(newHue) + "}";

	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url, body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	// string output = "";
	
	// // call 'put' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.putRequest(url, body);
		
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// return "";
}

void BridgeController::setGroupActionSat(Bridge bridge, int groupID, int newSat) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID) +"/action";
	
	string body = "{\"sat\":" + to_string(newSat) + "}";

	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url, body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	// string output = "";
	
	// // call 'put' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.putRequest(url, body);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// return "";
}

void BridgeController::setGroupActionBright(Bridge bridge, int groupID, int newBright) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID) +"/action";
	
	string body = "{\"bri\":" + to_string(newBright) + "}";


	BridgeConnector *http = new BridgeConnector();
	http->putRequest(url, body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
	// // call 'put' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.putRequest(url, body);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// return "";
}

void BridgeController::setGroupActionEffect(Bridge bridge, int groupID, int newEffect) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID) +"/action";
	
	string body = "";
	
	if (newEffect == 1) {
		body = "{\"effect\":\"colorloop\"}";
	} else {
		body = "{\"effect\":\"none\"}";
	}
		BridgeConnector *http = new BridgeConnector();
	http->putRequest(url, body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
// 	string output = "";
	
// 	// call 'put' request from BridgeConnector
// 	BridgeConnector bridgeConnector;
// 	output = bridgeConnector.putRequest(url, body);
	
// 	if (errorCode(output)) {
// 		return errorDescription(errorCode(output));
// 	}
	
// 	return "";
// 
}

void BridgeController::createGroup(Bridge bridge, string name) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/";
	
	string body = "{\"lights\":[],\"name\": \""+name+"\", \"type\":\"LightsGroup\"}";
	//{"lights":[],"name":"test group", "type":"LightsGroup"}
	
	
	/*
	for (unsigned int i = 0; i < lights.size(); i ++) {
		body += "\"" + to_string(lights[i])  + "\"";
		if(i < (lights.size() - 1))
			body += ",";
	}
	*/

	BridgeConnector *http = new BridgeConnector();
	http->postRequest(url, body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	
	// string output = "";
	
	// // call 'post' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.postRequest(url, body);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// return "";
}

void BridgeController::createGroup(Bridge bridge, vector<int> lights, string name) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/";
	
	string body = "{\"lights\":[";
	
	for (unsigned int i = 0; i < lights.size(); i ++) {
		body += "\"" + to_string(lights[i])  + "\"";
		if(i < (lights.size() - 1))
			body += ",";
	}
	
	body += "], \"name\":" + name + "\"type\":\"LightsGroup\"}";

	BridgeConnector *http = new BridgeConnector();
	http->postRequest(url, body);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	// string output = "";
	
	// // call 'post' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.postRequest(url, body);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// return "";
}

void BridgeController::deleteGroup(Bridge bridge, int groupID) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/groups/" + to_string(groupID);

	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	// string output = "";
	
	// // call 'delete' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.deleteRequest(url);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// return "";
}

void BridgeController::getAllSchedules(Bridge bridge) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/schedules";
	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));	
	// string output = "";

	// // call 'get' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.getRequest(url);
	
	// if (errorCode(output)) {
	// 	vector<string> outErr;
		
	// 	outErr.push_back(errorDescription(errorCode(output)));
	// 	outErr.push_back("0");
		
	// 	return outErr;
	// }
	
	// Wt::Json::Object jsonObj;
	// Wt::Json::parse(output, jsonObj);
	
	// set<string> idSet = jsonObj.names();
	// vector<string> idVec(idSet.begin(), idSet.end());
	
	// idVec.push_back("1");
	
	// return idVec;
}

void BridgeController::getScheduleName(Bridge bridge, int schedID) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/schedules/" + to_string(schedID);
	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));

	// string output = "";
	
	// // call 'get' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.getRequest(url);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// Wt::Json::Object jsonObj;
	// Wt::Json::parse(output, jsonObj);
	
	// if (jsonObj.get("name").type() == Wt::Json::NullType) {
	// 	return "Name: ";
	// }
	
	// string name = jsonObj.get("name");
	
	// return "Name: " + name;
}

void BridgeController::getScheduleTime(Bridge bridge, int schedID) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/schedules/" + to_string(schedID);
	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));	
	// string output = "";
	
	// // call 'get' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.getRequest(url);
	
	// if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// Wt::Json::Object jsonObj;
	// Wt::Json::parse(output, jsonObj);
	
	// if (jsonObj.get("time").type() == Wt::Json::NullType) {
	// 	return "Time: ";
	// }
	
	// string localTime = jsonObj.get("time");
	
	// return "Time: " + localTime;
}

void BridgeController::getScheduleCommand(Bridge bridge, int schedID, int command) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/schedules/" + to_string(schedID);
	BridgeConnector *http = new BridgeConnector();
	http->getRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));

	// string output = "";
	
	// // call 'get' request from BridgeConnector
	// BridgeConnector bridgeConnector;
	// output = bridgeConnector.getRequest(url);
	
	// /*if (errorCode(output)) {
	// 	return errorDescription(errorCode(output));
	// }
	
	// Wt::Json::Object jsonObj;
	// Wt::Json::parse(output, jsonObj);
	
	// Wt::Json::Object commandObj = jsonObj.get("command");
	
	// string address = commandObj.get("address");
	
	// bool isGroup = false;
	// size_t idPos = string::npos;
    
	// if (address.find("/groups/") != string::npos) {
	// 	isGroup = true;
    
	// 	idPos = address.find("/groups/") + 8;
	// } else {
	// 	idPos = address.find("/lights/") + 8;
	// }
    
	// string idStr = "";
	// int id = 0;
    
	// while (idPos < address.size()) {
 //        if (address.at(idPos) == '/') {
 //            break;
 //        }
 //        idStr.append(string(1, address.at(idPos)));
	// 	idPos++;
	// }
    
	// id = stoi(idStr);
	
	// Wt::Json::Object bodyObj = commandObj.get("body");
	
	// switch (command) {
	// 	case 0:			// type
	// 		if (isGroup) {
	// 			return "Type: Group";
	// 		} else {
	// 			return "Type: Light";
	// 		}
	// 	case 1:			// id
	// 		return "ID: " + to_string(id);
	// 	case 2: {		// on
	// 		if (bodyObj.get("on").type() == Wt::Json::NullType) {
	// 			return "On: ";
	// 		}
			
	// 		bool isOn = bodyObj.get("on");
			
	// 		if (isOn) {
	// 			return "On: True";
	// 		} else {
	// 			return "On: False";
	// 		}
	// 	}
	// 	case 3:	{		// hue
	// 		if (bodyObj.get("hue").type() == Wt::Json::NullType) {
	// 			return "Hue: ";
	// 		}
			
	// 	 	int hue = bodyObj.get("hue");
			
	// 		return "Hue: " + to_string(hue);
	// 	}
	// 	case 4:	{		// sat
	// 		if (bodyObj.get("sat").type() == Wt::Json::NullType) {
	// 			return "Saturation: ";
	// 		}
			
	// 		int sat = bodyObj.get("sat");
			
	// 		return "Saturation: " + to_string(sat);
	// 	}
	// 	case 5: {		// bri
	// 		if (bodyObj.get("bri").type() == Wt::Json::NullType) {
	// 			return "Brightness: ";
	// 		}
			
	// 		int bri = bodyObj.get("bri");
			
	// 		return "Brightness: " + to_string(bri);
	// 	}
	// 	case 6:	{		// effect
	// 		if (bodyObj.get("effect").type() == Wt::Json::NullType) {
	// 			return "Effect: None";
	// 		}
			
	// 		string effect = bodyObj.get("effect");
			
	// 		if (effect.at(0) == 'c') {
	// 			return "Effect: Color Loop";
	// 		} else {
	// 			return "Effect: None";
	// 		}
	// 	}
	// 	default:
	// 		//return "No command element chosen.";
	// }

}

void BridgeController::setSchedule(Bridge bridge, int schedID, string name, string localTime, int ID, int newHue, int newSat,
	 int newBright, bool isOn, int effect, bool isGroup, bool updateExisting) {
	
	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/schedules";
	
	if (updateExisting) {
		url.append("/" + schedID);
	}
	
	string body = "{";
	
	if (name != "") {
		body.append("\"name\":\"" + name + "\",");
	}
	
	body.append("\"command\":{\"address\":\"/api/" + bridge.getUserName());

	if (!isGroup) {
		body.append("/lights/" + to_string(ID) + "/state\",");
	} else {
		body.append("/groups/" + to_string(ID) + "/action\",");
	}
	
	body.append("\"method\":\"PUT\",");
	
	body.append("\"body\":{");
	
	if (isOn == true) {
		if (newHue >= 0) {
			body.append("\"hue\":" + to_string(newHue) + ",");
		}
		
		if (newSat >= 0) {
			body.append("\"sat\":" + to_string(newSat) + ",");
		}
		
		if (newBright >= 1) {
			body.append("\"bri\":" + to_string(newBright) + ",");
		}
		
		if (effect == 1) {
			body.append("\"effect\":\"colorloop\",");
		} else {
			body.append("\"effect\":\"none\",");
		}
		
		body.append("\"on\":true");
	} else {
		body.append("\"on\":false");
	}
	
	body.append("}},\"time\":\"" + localTime + "\"}");
	
	// string output = "";
	
	if (!updateExisting) {
		BridgeConnector *http = new BridgeConnector();
		http->postRequest(url,body);
		http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
	} else {
		BridgeConnector *http = new BridgeConnector();
		http->putRequest(url,body);
		http->done().connect(boost::bind(&BridgeController::return_json,this,_1));		
		// // call 'put' request from BridgeConnector
		// BridgeConnector bridgeConnector;
		// output = bridgeConnector.putRequest(url, body);
	}
	
	// if (errorCode(output)) {
	// 	 errorDescription(errorCode(output));
	// }
	
	// //return "";
}
 void BridgeController::deleteSchedule(Bridge bridge, int schedID) {
	
 	string url = "http://" + bridge.getIpAddress() + ":" + bridge.getPort() + "/api/" + bridge.getUserName() + "/schedules/" + to_string(schedID);
	
	string output = "";
	BridgeConnector *http = new BridgeConnector();
	http->deleteRequest(url);
	http->done().connect(boost::bind(&BridgeController::return_json,this,_1));
}


void BridgeController::return_json(string jsonString){

	if(jsonString.back() == ']'){
		jsonString.erase(jsonString.size()-1);
		std::cout<<"cahnged last   "<<jsonString<<"   ffffffffff"<<std::endl;
	}
	if(jsonString.at(0) =='['){
		jsonString.erase(0, 1);
		std::cout<<"cahnged first  "<<jsonString<<"   ffffffffff"<<std::endl;

	}
	
	std::cout<<"fffffffff"<<jsonString<<"ffffffffff"<<std::endl;
	done1_.emit(jsonString);
	std::cout << "GSIGNAL EMITTED" << std::endl;
}


Wt::Signal<std::string>& BridgeController::done1() { return done1_; }
