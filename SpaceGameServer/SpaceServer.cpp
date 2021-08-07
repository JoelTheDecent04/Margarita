#include <enet/enet.h>
#include <sstream>
#include <chrono>
#include "Space.h"
#include "SpaceServer.h"
#include "Player.h"

bool DoEvents();

std::string ip_representation(uint32_t ip)
{
	std::stringstream ss;

	ss << (ip & 0xff) << '.' << ((ip & 0xff00) >> 8) << '.' << ((ip & 0xff0000) >> 16) << '.' << ((ip & 0xff000000) >> 24);

	return ss.str();
}

void SendData(const nlohmann::json& j, ENetPeer* peer)
{
	std::stringstream ss;
	ss << j;
	std::string s = ss.str();

	ENetPacket* packet = enet_packet_create(s.c_str(), s.length() + 1, ENET_PACKET_FLAG_RELIABLE);
	int ret = enet_peer_send(peer, 0, packet);
	assert(ret == 0);
	//TODO delete packet

}

void BroadcastData(const nlohmann::json& j, ENetHost* host)
{
	std::stringstream ss;
	ss << j;
	std::string s = ss.str();

	ENetPacket* packet = enet_packet_create(s.c_str(), s.length() + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(host, 0, packet);
	//TODO delete packet
}

nlohmann::json GetData(ENetPacket* packet)
{
	return nlohmann::json::parse((const char*)packet->data);
}

void SpaceServerMain()
{
	auto game = std::make_shared<SpaceGameServer>();
	sgSpaceGame = game;
	game->vPlayers.push_back(std::make_shared<Player>(384.0f, 380.f, "Player0"));

	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = 5123;

	ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
	assert(server);

	int event_status;

	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

	float fPacketInterval = 1.0f / 30.0f;
	float fSecondsSinceLastPacket = 0.0f;

	ENetEvent event;
	while (1)
	{
		std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
		auto time = std::chrono::duration<float, std::milli>(current_time - start_time) / 1000.0f;
		float fDeltaTime = time.count() / 1000.0f;

		start_time = current_time;

		//Receive packets
		event_status = enet_host_service(server, &event, 0);

		if (event_status > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("New connection from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("Message from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s:%d disconnected\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
				event.peer->data = nullptr;
				break;
			}
		}

		fSecondsSinceLastPacket -= fDeltaTime;
		if (fSecondsSinceLastPacket <= 0.0f)
		{
			fSecondsSinceLastPacket = fPacketInterval + fSecondsSinceLastPacket;

			BroadcastData(game->GetState(), server);
		}


		game->Update(fDeltaTime);

		Sleep(10);
	}
}