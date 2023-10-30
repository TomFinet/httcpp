#include <string>
#include <iostream>
#include <stdexcept>

#include <uri/uri.h>
#include <uri/uri_error.h>

int main()
{
    uri::uri uri;
    std::string host, path;

    path = "/d/";
    try {
        uri.parse_path(path.begin(), path.end());
    }
    catch (uri::uri_error &err) {
        std::cout << err.what() << std::endl;
    }
    catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
}