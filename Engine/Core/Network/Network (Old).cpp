#include "Network.h"
#include "Core.h"

#define NETWORK_PORT (30001)

namespace Core
{
		

	Network::Network()
	{
		State = NetworkConnectionState::NotInUse;

		Host();
	}

	Network::~Network()
	{
	}

	void Network::Host()
	{
		host_thread = new std::thread([this]()
		{
			try
			{
				using boost::asio::ip::tcp;
				boost::asio::io_service io_service;
				tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), NETWORK_PORT));

				Debug::Log("Server Listening...");

				for (;;)
				{
					tcp::socket socket(io_service);
					acceptor.accept(socket);

					std::string message = "Message sent from server.";

					boost::system::error_code ignored_error;
					boost::asio::write(socket, boost::asio::buffer(message), ignored_error);

					Debug::Log("Server sent message...");
				}
			}
			catch (std::exception& e)
			{
				Debug::Log(e.what());
			}
		});	
	}

	void Network::Join(const std::string& address)
	{
		if (State != NetworkConnectionState::NotInUse)
		{
			return;
		}
		State = NetworkConnectionState::ClientConnecting;
		
		client_thread = new std::thread([this, &address]()
		{
			try
			{
				using boost::asio::ip::tcp;
				boost::asio::io_service io_service;
				tcp::resolver resolver(io_service);
				tcp::resolver::query query(address, std::to_string(NETWORK_PORT));
				tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
				tcp::socket socket(io_service);
				boost::asio::connect(socket, endpoint_iterator);

				for (;;)
				{
					std::vector<char> buf(180, '\0');
					boost::system::error_code error;

					size_t len = socket.read_some(boost::asio::buffer(buf), error);

					if (error == boost::asio::error::eof)
						break; // Connection closed cleanly by peer.
					else if (error)
						throw boost::system::system_error(error); // Some other error.

					std::string input(buf.data());
					Debug::Log(input);
				}
			}
			catch (std::exception& e)
			{
				Debug::Log(e.what());
			}
		});
	}

	void Network::Disconnect()
	{
		if (State == NetworkConnectionState::HostInitializing ||
			State == NetworkConnectionState::Hosting)
		{
			//delete Server;
		}

		if (State == NetworkConnectionState::ClientInitializing ||
			State == NetworkConnectionState::ClientConnecting ||
			State == NetworkConnectionState::ClientConnected)
		{
			//delete Client;
		}

		// destroy thread

		State = NetworkConnectionState::NotInUse;
	}
}