#include <string>

#include "Bridge.h"

using namespace std;

Bridge::Bridge(string setName, string setIpAddress, string setPort,
	string setUserName) {
	
	name = setName;
	ipAddress = setIpAddress;
	port = setPort;
	userName = setUserName;
}

Bridge::Bridge(string setName, string setIpAddress, string setPort,
	string setUserName, string setLocation) {
	
	name = setName;
	ipAddress = setIpAddress;
	port = setPort;
	userName = setUserName;
	location = setLocation;
}

Bridge::Bridge() {}

Bridge::~Bridge() {}

string Bridge::getName() { return name; }
string Bridge::getIpAddress() { return ipAddress; }
string Bridge::getPort() { return port; }
string Bridge::getUserName() { return userName; }
string Bridge::getLocation() { return location; }
int Bridge::getID(){return bridgeID; }

void Bridge::setName(string newName) { name = newName; }
void Bridge::setIpAddress(string newIpAddress) { ipAddress = newIpAddress; }
void Bridge::setPort(string newPort) { port = newPort; }
void Bridge::setUserName(string newUserName) { userName = newUserName; }
void Bridge::setLocation(string newLocation) { location = newLocation; }
