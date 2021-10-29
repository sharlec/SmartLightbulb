/**
 * @author Chris Steward
 * @brief Class for actually sending and receiving the http requests from the bridge controller
 * @date November 25, 2017
 */

#include "BridgeConnector.h"

/**
 * @brief BridgeConnector Constructor
 * @returns Returns a BridgeConnector object
 */   
BridgeConnector::BridgeConnector(){}

/**
 * @brief BridgeConnector Destructor
 * @returns N/A
 */  
BridgeConnector::~BridgeConnector(){}

 /**
 * @brief 
 * @param
 * @returns 
 */   
std::string BridgeConnector::getRequest(std::string url)
{
	Wt::Http::Client *client = new Wt::Http::Client();
	client->setTimeout(15);
	client->setMaximumResponseSize(10 * 1024);
	std::string output;
	client->done().connect(boost::bind(&BridgeConnector::handleHttpRequest, this, _1, _2));
	if(client->get(url)){

	}
	else {
		std::cout << "Error: bad URL given to BridgeConnector::getRequest" << std::endl;
	}
	return output;
}

 /**
 * @brief 
 * @param
 * @returns 
 */   
std::string BridgeConnector::postRequest(std::string url, std::string body)
{
	Wt::Http::Client *client = new Wt::Http::Client();
	client->setTimeout(15);
	client->setMaximumResponseSize(10 * 1024);
	std::string output;

	Wt::Http::Message message = Wt::Http::Message();
	message.addBodyText(body);
	
	client->done().connect(boost::bind(&BridgeConnector::handleHttpRequest, this, _1, _2));
	if(client->post(url, message)){

	}
	else {
		std::cout << "Error: bad URL given to BridgeConnector::postRequest" << std::endl;
	}
	return output;
}

 /**
 * @brief 
 * @param
 * @returns 
 */  
std::string BridgeConnector::putRequest(std::string url, std::string body)
{
	Wt::Http::Client *client = new Wt::Http::Client();
	client->setTimeout(15);
	client->setMaximumResponseSize(10 * 1024);
	std::string output;

	Wt::Http::Message message = Wt::Http::Message();
	message.addBodyText(body);
	
	client->done().connect(boost::bind(&BridgeConnector::handleHttpRequest, this, _1, _2));
	if(client->put(url, message)){

	}
	else {
		std::cout << "Error: bad URL given to BridgeConnector::putRequest" << std::endl;
	}
	return output;
}

 /**
 * @brief 
 * @param
 * @returns 
 */  
std::string BridgeConnector::deleteRequest(std::string url)
{
	Wt::Http::Client *client = new Wt::Http::Client();
	client->setTimeout(15);
	client->setMaximumResponseSize(10 * 1024);
	std::string output;

	Wt::Http::Message message = Wt::Http::Message();

	client->done().connect(boost::bind(&BridgeConnector::handleHttpRequest, this, _1, _2));
	if(client->deleteRequest(url, message)){

	}
	else {
		std::cout << "Error: bad URL given to BridgeConnector::deleteRequest" << std::endl;
	}
	return output;	
}

 /**
 * @brief 
 * @param
 * @returns 
 */  

void BridgeConnector::handleHttpRequest(boost::system::error_code err, const Wt::Http::Message &response)
{
	Wt::WApplication::instance()->resumeRendering();
	std::string output;	
	std::cout << "REACHED bconnector HANDLER*************************************" << std::endl;
	if (response.status() >= 200) {
		std::cout << "REACHED if !ERR*************************************" << output << std::endl;
		output = response.body();
	}
	done_.emit(output);
}





Wt::Signal<std::string>& BridgeConnector::done() { return done_; }
