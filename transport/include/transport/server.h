/* Class wrapping the IO queue used to send and receive packets.
   On windows this creates an IO completion port. */

#pragma once

#include <stdexcept>
#include <vector>
#include <mutex>
#include <memory>
#include <functional>
#include <thread>

#include <transport/platform.h>
#include <transport/conn_ctx.h>
#include <transport/acceptor.h>
#include <transport/socket.h>
#include <transport/config.h>

#include <threadpool/pool.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace transport
{

    using io_cb_t = std::function<void(socket_t)>;
    using conn_cb_t = std::function<void(std::shared_ptr<conn_ctx>)>;

    class server {
         
        private:
            conn_cb_t on_conn_;
            io_cb_t on_rx;
            io_cb_t on_tx;
            io_cb_t on_client_close;
            io_queue_t queue_handle_;

            acceptor acc_;

            std::unique_ptr<threadpool::pool> io_workers;

            struct config config_; 
            std::shared_ptr<spdlog::logger> logger_;

            std::unordered_map<socket_t, std::shared_ptr<conn_ctx>> conns_;
            std::mutex conn_mutex_;
            std::condition_variable conn_condition_;

        #ifdef __linux__
            std::unordered_map<socket_t, std::unique_ptr<epoll_event>> events_;
            std::unordered_map<socket_t, std::unique_ptr<io_ctx>> ongoing_io;
        #endif

            void register_socket(const socket_t handle, const conn_ctx *const conn); 

        public:

            server(conn_cb_t on_conn, io_cb_t on_rx, io_cb_t on_tx, io_cb_t on_client_close, const config &config);

            ~server()
            {
                transport::socket::cleanup();
                /* TODO: we probs need to close the queue_handle file. */
            }

            void start(void)
            {
                for (int i = 0; i < config_.num_req_handler_threads; i++) {
                    io_workers->submit([this]()
                    {
                        try {
                            this->dequeue();
                        }
                        catch (std::exception &ex) {
                            this->logger_->error(ex.what());
                        }
                    });
                }

                /* start listening for incoming connections. */
                acc_.open(config_.listening_ip, config_.listening_port, config_.max_connection_backlog,
                          config_.max_linger_sec, config_.rx_buf_len);
                        
                while (true) {
                    {
                        std::unique_lock<std::mutex> lock(conn_mutex_);
                        conn_condition_.wait(lock, [this]{ return conns_.size() < config_.max_concurrent_connections; });
                    }
                    transport::socket_t skt_handle = acc_.accept();

                    std::shared_ptr<conn_ctx> conn = std::make_shared<conn_ctx>(skt_handle,
                        [this](socket *const skt) { this->rx(skt); },
                        [this](socket *const skt, std::string_view msg) { this->tx(skt, msg);
                    });

                    logger_->info("Connections: {0:d}", conns_.size());

                    add_conn(conn);
                    register_socket(skt_handle, conn.get());
                    logger_->info("New connection established.");
                    on_conn_(conn);
                }
            }

            void dequeue(void);

            void rx(socket *const skt);
            void tx(socket *const skt, std::string_view msg);
            
            void add_conn(std::shared_ptr<conn_ctx> conn)
            {
                std::unique_lock<std::mutex> lock(conn_mutex_);
                conns_[conn->skt->handle()] = conn;
            }

            void remove_conn(transport::socket_t skt_handle)
            {
                std::unique_lock<std::mutex> lock(conn_mutex_);
                conns_.erase(skt_handle);
                conn_condition_.notify_one();
            }
    };

}