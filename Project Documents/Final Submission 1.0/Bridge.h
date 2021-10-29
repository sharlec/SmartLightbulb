#ifndef BRIDGE_H
#define BRIDGE_H

#include <string>

class Bridge {
    
    public:
        // constructor and destructor
        Bridge(std::string setName, std::string setIpAddress, std::string setPort,
            std::string setUserName);
        Bridge(std::string setName, std::string setIpAddress, std::string setPort,
            std::string setUserName, std::string setLocation);
        Bridge();
        ~Bridge();
        
        //getters
        std::string getName();
        std::string getIpAddress();
        std::string getPort();
        std::string getUserName();
        std::string getLocation();
        int getID();

        //setters
        void setName(std::string newName);
        void setIpAddress(std::string newIpAddress);
        void setPort(std::string newPort);
        void setUserName(std::string newUserName);
        void setLocation(std::string newLocation);
        void setID(int newBridge);

    private:
        int bridgeID;
        std::string name;
        std::string ipAddress;
        std::string port;
        std::string userName;
        std::string location;
};

#endif
