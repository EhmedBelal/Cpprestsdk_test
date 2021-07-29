#include <iostream>
#include <filesystem>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

namespace fs = std::experimental::filesystem::v1;

int main(int argc, char* argv[])
{
    uri link = uri(U("ws://echo.websocket.org"));
    uri uriGeoData = uri(L"https://dev-digitaltwin.azurewebsites.net/api/ifc/GetHierarchyTree");

    // Make the request and asynchronously process the response.
    http_client client(L"https://dev-digitaltwin.azurewebsites.net/api/ifc/GetHierarchyTree");

    client.request(methods::GET).then([](http_response response) {
        if (response.status_code() == status_codes::OK) {
            auto body = response.extract_string().get();
            std::wcout << body << std::endl;
            response.extract_json();
        }});

    client.request(methods::GET).then([](http_response response) -> pplx::task<json::value>
        {
            std::wostringstream ss;
            ss << L"Server returned returned status code " << response.status_code() << L"." << std::endl;
            std::wcout << ss.str();
            //return pplx::task_from_result(json::value());
            return response.extract_json();
        })
        .then([](pplx::task<json::value> previousTask)
            {
                try
                {
                    const json::value& v = previousTask.get();
                    // Perform actions here to process the JSON value...
                    
                    auto strId = v.at(U("Id")).as_string();
                    std::wcout << "Id : " << strId << std::endl;
                }
                catch (const http_exception& e)
                {
                    // Print error.
                    std::wostringstream ss;
                    ss << e.what() << std::endl;
                    std::wcout << ss.str();
                }
            });
        std::cout << "GET geo data in JSON" << std::endl;
        system("PAUSE");
        return 0;
}