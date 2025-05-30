#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifyGrpcClient.h"

void LogicSystem::RegGet(std::string url, HttpHandler handler)
{
	_get_handlers.insert(make_pair(url, handler));
}
void LogicSystem::RegPost(std::string url, HttpHandler handler)
{
	_post_handlers.insert(make_pair(url, handler));
}
LogicSystem::LogicSystem()
{
	RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection)
		   {
		beast::ostream(connection->_response.body()) << "recevie get_test req";
		int i = 0;
		for (auto& elem : connection->_get_params)
		{
			i++;
			beast::ostream(connection->_response.body()) << "param is " << i << "key is " << elem.first;
			beast::ostream(connection->_response.body()) << "param is " << i << "value is " << elem.second << std::endl;
		} });
	RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection)
			{
		auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
		connection->_response.set(http::field::content_type, "text/json");
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success)
		{
			std::cout << "Failed to parse Json data " << std::endl;
			root["error"] = ErrorCode::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}
		if (!src_root.isMember("email"))
		{
			std::cout << "Failed to parse Json data " << std::endl;
			root["error"] = ErrorCode::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}
		auto email = src_root["email"].asString();
		GetVarifyRsp rsp = VerifyGrpcClient::GetInstance()->GetVarifyCode(email);
		std::cout << "email is " << email << std::endl;
		root["error"] = rsp.error();
		root["email"] = src_root["email"];
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->_response.body()) << jsonstr;
		return true; });
}
bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con)
{
	if (_get_handlers.find(path) == _get_handlers.end())
	{
		return false;
	}
	_get_handlers[path](con);
	return true;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con)
{
	std::cout << "HandlePost called with path: " << path << std::endl;
	if (_post_handlers.find(path) == _post_handlers.end())
	{
		return false;
	}
	std::cout << "Path found, invoking handler for: " << path << std::endl;
	_post_handlers[path](con);
	return true;
}