/*
 * Author: Jacob Stirling
 * Description: Header file for FileController.cpp
 * Date: November 10, 2017
 */

#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include <string>
#include <fstream>

#include "User.h"

class FileController {

	public:
		FileController();
		~FileController();
		
		bool userExists(std::string email);

		int getUser(User &user, std::string email, std::string hashedPassword);
		int saveUser(User &user);

		int createUser(User &user);
		int deleteUser(std::string email);

	private:
		size_t emailLocation(std::string email, std::string dataStr);
		std::string getEmailStr(std::string emailStr);
		
		std::string fileToString();
		void stringToFile(std::string dataStr);

		void markFileAccessable(bool accessable);
		bool isFileAccessable();
		std::string removeSemaphore(std::string dataStr);

		std::string readFromFile();
		void writeToFile(std::string dataStr);

};

#endif
