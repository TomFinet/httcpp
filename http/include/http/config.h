#pragma once

#include <string>


namespace http
{

    /* default server resource configuration. */
    constexpr unsigned int default_max_concurrent_connections = 2;
    constexpr unsigned int default_max_connection_backlog = 5000;
    constexpr unsigned int default_max_peer_req_per_min = 50;
    constexpr unsigned int default_req_timeout_ms = 2000;
    constexpr unsigned int default_idle_connection_timeout_ms = 10000;
    constexpr unsigned short default_max_linger_sec = 2;
    constexpr int default_num_req_handler_threads = 8;
    constexpr unsigned int default_rx_buf_len = 1024;

    /* default server network configuration. */
    constexpr unsigned int default_listening_port = 9001;
    constexpr auto &default_listening_ip = "127.0.0.1";
    constexpr auto &default_listening_reg_name = "localhost";

    /* default server message parsing configuration. */
    constexpr unsigned int default_max_req_line_len = 1000;
    constexpr auto &default_default_uri_scheme = "http";
    constexpr auto &default_default_req_content_type = "application/json";

    constexpr auto &default_logger_name = "http_server";

    constexpr auto &default_resource_search_path = "";


    /* stores the configuration options chosen by the server. */
    struct config {

        /* Maximum number of ms spent handling a request before a timeout. */
        unsigned int req_timeout_ms = default_req_timeout_ms;

        /* Maximum number of ms a connection can exist idle before it is closed. */
        unsigned int idle_connection_timeout_ms = default_idle_connection_timeout_ms;

        /* Maximum number of seconds to keep transport connection open after requesting to close it.
        DONE. */
        unsigned short max_linger_sec = default_max_linger_sec;

        /* Maximum number of concurrent connections. */
        unsigned int max_concurrent_connections = default_max_concurrent_connections;

        /* Maximum number of queued connections, after which subsequent ones are dropped.
        DONE. */ 
        unsigned int max_connection_backlog = default_max_connection_backlog;

        /* Maximum number of requests from same remote peer per minute. */
        unsigned int max_peer_req_per_min = default_max_peer_req_per_min;

        /* Number of threads for request handling.
        DONE. */
        int num_req_handler_threads = default_num_req_handler_threads;

        /* Size in bytes of the request receive buffers.
        DONE. */
        unsigned int rx_buf_len = default_rx_buf_len;

        /* Server listens for transport packets on this port. 
        DONE. */
        unsigned int listening_port = default_listening_port;

        /* Server listens for transport packets addressed to this ip.
        DONE. */
        std::string listening_ip = default_listening_ip;

        /* Server listens for transport packets addressed to this registered name.
        DONE. */
        std::string listening_reg_name = default_listening_reg_name;
    
        /* Maximum size of a request message's request line.
        DONE. */
        unsigned int max_req_line_len = default_max_req_line_len;

        /* The uri scheme to use by default when uri leaves the scheme unspecified. */
        std::string default_uri_scheme = default_default_uri_scheme;

        /* Default content type when request leaves it unspecified. */
        std::string default_req_content_type = default_default_req_content_type;

        std::string logger_name = default_logger_name;

        std::string resource_search_path = default_resource_search_path;
    };

    /* read-only configuration. */
    constexpr short supported_http_major_version = 1;
    constexpr short supported_http_minor_version = 1;

}