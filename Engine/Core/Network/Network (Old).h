#pragma once
#include <set>
#include <thread>

#include <boost/asio.hpp>

namespace Core
{
	enum class NetworkConnectionState : unsigned int {
		NotInUse,
		HostInitializing,
		Hosting,
		ClientInitializing,
		ClientConnecting,
		ClientConnected
	};
		

	class Network
	{
	public:
		Network();
		~Network();

		void Host();
		void Join(const std::string& address);
		void Disconnect();

	private:
		std::thread* host_thread;
		std::thread* client_thread;

		NetworkConnectionState State;
	};
}