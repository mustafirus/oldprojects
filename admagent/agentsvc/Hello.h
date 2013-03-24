#pragma once
//#include "p:\cpp\xmlrpc++0.7\src\xmlrpcservermethod.h"

class Hello : public XmlRpcServerMethod
{
public:
  Hello(XmlRpcServer* s) : XmlRpcServerMethod("Hello", s) {}

  void execute(XmlRpcValue& params, XmlRpcValue& result)
  {
    result = "Hello";
  }

  std::string help() { return std::string("Say hello"); }
};
