#ifndef BRIDGE_CONNECTOR_H
#define BRIDGE_CONNECTOR_H

#include <string>

#include <Wt/Http/Client>
#include <Wt/WApplication>

class BridgeConnector {
    
    public:
        BridgeConnector();
        ~BridgeConnector();
        
        std::string getRequest(std::string url);
        std::string postRequest(std::string url, std::string body);
        std::string putRequest(std::string url, std::string body);
        std::string deleteRequest(std::string url);

	
	void handleHttpRequest(boost::system::error_code err, const Wt::Http::Message &response);

	Wt::Signal<std::string>& done();

	
    private:
	Wt::Signal<std::string> done_;
        
};

#endif
