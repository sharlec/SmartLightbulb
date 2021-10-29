/*
 * Author: Jacob Stirling
 * Description: Methods to get/set user information to the data file (data.txt)
 * and delete/create users in the data file. Implements FileController.h.
 * Date: November 10, 2017
 */

#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <stack>

#include <Wt/Json/Object>
#include <Wt/Json/Array>
#include <Wt/Json/Value>
#include <Wt/Json/Parser>

#include "FileController.h"
#include "User.h"
#include "Bridge.h"

using namespace std;

// FileController default constructor
FileController::FileController() { }

// FileController default destructor
FileController::~FileController() { }

/*
 * Function name: useExists
 * Description: returns whether user exists in the data file
 * Parameter description: string email - user email
 * Return description: boolean value. true if user exists, false otherwise
 */
bool FileController::userExists(string email) {
	
	string dataStr = readFromFile();
	
	// if the email can be found in the data file, return true
	if (emailLocation(email, dataStr) != string::npos) {
		return true;
	} else {
		return false;
	}
}

/*
 * Function Name: getUser
 * Description: retrive user information from data file
 * Parameter descriptions: User &user - to contain retrieved invormation;
 * string email - entered email; string hashedPassword - entered password
 * after going through hashing algorithm
 * Return description: integer value (0, 1, or 2) - 0 if user was retrieved, 1
 * if email doesn't correspond to existing user, 2 if password is incorrect
 */
int FileController::getUser(User &user, string email,
	string hashedPassword) {

	string dataStr = readFromFile();

	// jsonObj will be JSON Object representation of the data file
	Wt::Json::Object jsonObj;

	// defines jsonObj
	Wt::Json::parse(dataStr, jsonObj);

	// if the email cannot be found in the data file, return 1
	if (jsonObj.get(email).type() == Wt::Json::NullType) {
		return 1;
	}
	
	// defines userObj to be a JSON Object representation of the user found by
	//	the given email
	const Wt::Json::Object& userObj = jsonObj.get(email);

	// gets the user's hashed password from userObj
	string jsonHashedPassword = userObj.get("hashedPassword");

	// if the entered password does not match the actual password, retrun 2
	if (jsonHashedPassword != hashedPassword) {
		return 2;
	}
	
	// gets the bridges from userObj
	Wt::Json::Array bridgeArr = userObj.get("bridges");
	
	unsigned int numBridges = bridgeArr.size();
	
	user.clearBridges();
	
	// adds the retrieved bridge information to the given user object
	for (unsigned int i = 0; i < numBridges; i++) {
		Bridge addBridge;
		
		Wt::Json::Object bridgeObj = bridgeArr.at(i);
		
		addBridge.setName(bridgeObj.get("name"));
		addBridge.setIpAddress(bridgeObj.get("ip"));
		addBridge.setPort(bridgeObj.get("port"));
		addBridge.setUserName(bridgeObj.get("username"));
		addBridge.setLocation(bridgeObj.get("location"));
		
		user.addToBridges(addBridge);
	}
	
	//	gets the rest of the user information from userObj
	string jsonFirstName = userObj.get("firstName");
	string jsonLastName = userObj.get("lastName");
	int jsonPin = userObj.get("pin");
	time_t jsonRegisterTime = atol(string(userObj.get("registerTime")).c_str());
	time_t jsonLastLogin = atol(string(userObj.get("lastLogin")).c_str());
	
	// adds the retrieved user information to the given user object
	user.setEmail(email);
	user.setFirstName(jsonFirstName);
	user.setLastName(jsonLastName);
	user.setHashedPassword(jsonHashedPassword);
	user.setPin(jsonPin);
	user.setRegisterTime(jsonRegisterTime);
	user.setLastLogin(jsonLastLogin);
	
	// a return value of 0 indicates that the user was retrieved from the data
	//	file
	return 0;
}

/*
 * Function name: saveUser
 * Description: updates the data file with the given user's updated information
 * Parameter description: User &user - existing user to update
 * Return description: error on -1, success on 1
 */
int FileController::saveUser(User &user) {

	if (user.empty()) {
	 	return-1;
	}

	string dataStr = readFromFile();

	// if the user cannot be found in the data file, exit from the function
	if (!userExists(user.getEmail())) {
		return-1;
	}

	// the next two lines update the user in the data file
	deleteUser(user.getEmail());
	createUser(user);
	return 1;
}

/*
 * Function name: createUser
 * Description: adds a new user to the data file
 * Parameter description: User &user - user to add to data file
 * Return description: error on -1, success on 0
 */
int FileController::createUser(User &user) {


	if (user.empty()) {
		return -1;
	}

	string dataStr = readFromFile();

	// if the user cannot be found in the data file, exit from the function
	if (userExists(user.getEmail())) {
		return -1;
	}

	string userStr = "";

	// generates a string representation of the given user (to be inserted into
	//	the data file)
	userStr.append(
		"\"" + user.getEmail() + "\":{"
			"\"firstName\":\"" + user.getFirstName() + "\","
			"\"lastName\":\"" + user.getLastName() + "\","
			"\"hashedPassword\":\"" + user.getHashedPassword() + "\","
			"\"pin\":" + to_string(user.getPin()) + ","
			"\"registerTime\":\"" + to_string(user.getRegisterTime()) + "\","
			"\"lastLogin\":\"" + to_string(user.getLastLogin()) + "\","
			"\"bridges\":["
	);

	unsigned int numBridges = user.getBridgeSize();
	
	// generates a string representation of bridge information (to be inserted
	//	into the data file)
	for (int i =0; i < numBridges; i++) {
		Bridge currentBridge = user.getBridge(i);
		
		userStr.append( "{"
			"\"name\":\"" + currentBridge.getName() + "\","
			"\"ip\":\"" + currentBridge.getIpAddress() + "\","
			"\"port\":\"" + currentBridge.getPort() + "\","
			"\"username\":\"" + currentBridge.getUserName() + "\","
			"\"location\":\"" + currentBridge.getLocation() + "\""
		"}"
		);
		
		if (i != numBridges - 1) {
			userStr.append(",");
		}
	}

	userStr.append("]}");

	// currentChar points to '\0' at the end of dataStr
	string::iterator currentChar;
	currentChar = dataStr.end();

	// loop while the iterator is not at the beginning of the string
	while ((currentChar - dataStr.begin()) != 0) {
		if (*currentChar == '}') {
			// calculate the position that the iterator is pointing to
			size_t currentPos = currentChar - dataStr.begin();
			
			dataStr.insert(currentPos, userStr);
			
			// if the data file was empty before userStr was inserted (i.e. the
			//	data file looked like {}), do not insert a comma before userStr,
			//	otherwise insert a comma (to separate user elements)
			if (dataStr.at(currentPos - 1) != '{') {
				dataStr.insert(currentPos, ",");
			}
			
			break;
		}
		
		// move the iterator back by one character
		currentChar--;
	}

	writeToFile(dataStr);
	
	return 0;
}

/*
 * Function name: deleteUser
 * Description: deletes a user from the data file associated with given email
 * Parameter description: string email - email associated with user to delete
 * Return description: error on -1, success on 1
 */
int FileController::deleteUser(string email) {

	string dataStr = readFromFile();

	size_t emailLoc = emailLocation(email, dataStr);

	// if the email cannot be found in the data file, exit from the function
	if (!userExists(email)) {
		return -1;
	}

	// include the " before the email
	emailLoc--;

	// currentPos will be iterating through dataStr until the end of the user
	//	object in dataStr
	int currentPos = emailLoc;

	char currentChar = dataStr.at(currentPos);

	// iterate forward until currentPos is pointing to the last } at the end of
	//	the user object in dataStr or until currentPos reaches the end of the
	//	string
	while((unsigned int)currentPos < dataStr.size()) {
		if (currentChar == '{') {
			stack<int> testStack;
			testStack.push(1);

			while((unsigned int)currentPos < dataStr.size()) {
				// move forward by one character
				currentPos++;
				currentChar = dataStr.at(currentPos);

				if (currentChar == '{') {
					testStack.push(1);
				} else if (currentChar == '}') {
					testStack.pop();
				}

				if (testStack.empty() == true) {
					break;
				}
			}
			break;
		}

		// move forward one character
		currentPos++;
		currentChar = dataStr.at(currentPos);
	}

	// to prevent the removal of EOS
	if ((unsigned int)currentPos >= dataStr.size()) {
		currentPos--;
	}

	int eraseLength = (currentPos - emailLoc) + 1;

	// if there is a comma just after the user object, remove it
	if (dataStr.at(currentPos + 1) == ',') {
		eraseLength++;
	}

	dataStr.erase(emailLoc, eraseLength);

	// if the user object is at the end of the dataStr, remove the comma just
	//	before it
	if (dataStr.at(emailLoc) == '}') {
		emailLoc -= 1;
		dataStr.erase(emailLoc, 1);
	}

	writeToFile(dataStr);
	return 1;
}

/*
 * Function name: emailLocation
 * Description: private helper function to locate email in data file
 * Parameter descriptions: string email - email to find; string dataStr -
 * string representation of the data file
 * Return description: size_t value - character location of first letter in
 * found email (size_t is an integer)
 */
size_t FileController::emailLocation(string email, string dataStr) {
	
	return dataStr.find(getEmailStr(email));
}

/*
 * Function name: getEmailStr
 * Description: private helper function to append ":{ to the end of a given
 * email so that the email to be found by the emailLocation function is not in
 * a member variable (e.g. so it won't accidentally return the email found in
* "firstName":"email@email.com")
 * Parameter descriptions: string emailStr - the email to be modified
 * Return description: a string - the modified email
 */
string FileController::getEmailStr(string emailStr) {
	
	string modifiedEmailStr = "";
	
	modifiedEmailStr.append(emailStr + "\":{");
	
	return modifiedEmailStr;
}

/*
 * Function name: fileToString
 * Description: creates a string out of the data file
 * Parameter description: N/A
 * Return description: a string - the string representation of the data file
 */
string FileController::fileToString() {

	string dataStr = "";

	string line = "";

	fstream file;
	file.open("data.txt");

	while(getline(file, line)) {
		dataStr.append(line);
	}

	file.close();

	return dataStr;
}

/*
 * Function name: stringToFile
 * Description: inserts (overwrites) the data file with a given string
 * Parameter description: string dataStr - the string to insert into the data
 * file
 * Return description: N/A
 */
void FileController::stringToFile(string dataStr) {

	// the next two lines delete the data file and recreate a blank one
	remove("data.txt");
	ofstream file("data.txt");
    
	file << dataStr;
    
	file.close();
}

/*
 * Function name: markFileAccessable
 * Description: marks the data file with a semophore if in use to prevent it
 * from being used by other processes or removes the semophore if the current
 * process is done accessing the file
 * Parameter description: bool accessable - if false, the data file will be
 * marked as inaccessable, if true, the data file will be unmarked
 * Return description: N/A
 */
void FileController::markFileAccessable(bool accessable) {

	string dataStr = fileToString();

	if(accessable) {
		dataStr = removeSemaphore(dataStr);
	} else {
		// the semophore is a 0 inserted at the beginning of the dataStr
		dataStr.insert(0, "0");
	}

	stringToFile(dataStr);
}

/*
 * Function name: isFileAccessable
 * Description: checks if the data file is inaccessable (ie. another process
 * is already accessing it)
 * Parameter description: N/A
 * Return description: boolean value - true if the data file is accessable,
 * false if not
 */
bool FileController::isFileAccessable() {

	string dataStr = fileToString();

	if (dataStr == "") {
		return false;
	}

	string semaphore;

	// get the first character in dataStr
	semaphore = dataStr.substr(0, 1);

	if(semaphore.compare("0") == 0) {
		return false;
	}

	return true;
}

/*
 * Function name: removeSemaphore
 * Description: removes the semaphore that marks a file as inaccessable
 * Parameter description: string dataStr - string representation of the data
 * file
 * Return description: a string - dataStr without the semaphore
 */
string FileController::removeSemaphore(string dataStr) {

	string checkCharacter;

	// removes all characters before the first {
	while(dataStr.length() != 0) {
		checkCharacter = dataStr.substr(0, 1);

		if(checkCharacter.compare("{") != 0) {
			dataStr.erase(0, 1);
		} else {
			break;
		}
	}

	return dataStr;
}

/*
 * Function name: readFromFile
 * Description: calls fileToString only if the file is accessable
 * Parameter description: N/A
 * Return description: a string - string representation of the data file
 */
string FileController::readFromFile() {
	while(!isFileAccessable());
	
	markFileAccessable(false);
	string dataStr = fileToString();
	markFileAccessable(true);

	dataStr = removeSemaphore(dataStr);

	return dataStr;
}

/*
 * Function name: writeToFile
 * Description: calls writeToFile only if the file is accessable
 * Parameter description: string dataStr - the string to insert into the data
 * file
 * Return description: N/A
 */
void FileController::writeToFile(string dataStr) {
	while(!isFileAccessable());
	
	markFileAccessable(false);
	stringToFile(dataStr);
	markFileAccessable(true);
}

