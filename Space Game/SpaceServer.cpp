#include <SDL.h>
#include <enet/enet.h>
#include <sstream>
#include "Space.h"
#include "Game.h"
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
	auto game = std::make_shared<SpaceGame>(SpaceGame::SERVER);
	Game::sgSpaceGame = game;
	game->plPlayer = std::make_shared<Player>(384.0f, 380.f, "Player0");

	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = 5123;

	ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
	assert(server);

	int event_status;

	uint64_t nPerformanceFrequency = SDL_GetPerformanceFrequency();
	float dSecondsPerCount = 1.0f / nPerformanceFrequency;

	uint64_t nStartTime = SDL_GetPerformanceCounter();

	float fPacketInterval = 1.0f / 30.0f;
	float fSecondsSinceLastPacket = 0.0f;

	ENetEvent event;
	while (1)
	{
		uint64_t nCurrentTime = SDL_GetPerformanceCounter();
		float dDeltaTime = (nCurrentTime - nStartTime) * dSecondsPerCount;
		nStartTime = nCurrentTime;

		//Receive packets
		event_status = enet_host_service(server, &event, 0);

		if (event_status > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				SDL_Log("New connection from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				SDL_Log("Message from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				SDL_Log("%s:%d disconnected\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
				event.peer->data = nullptr;
				break;
			}
		}

		fSecondsSinceLastPacket -= dDeltaTime;
		if (fSecondsSinceLastPacket <= 0.0f)
		{
			fSecondsSinceLastPacket = fPacketInterval + fSecondsSinceLastPacket;

			BroadcastData(game->GetState(), server);
		}


		game->Update(dDeltaTime);

		Sleep(10);
	}
}

OnlineSpaceGame::OnlineSpaceGame()
	: SpaceGame(SpaceGame::LOCAL)
{
	auto pSpaceGame = std::make_shared<SpaceGame>();
	Game::sgSpaceGame = pSpaceGame;


	client = enet_host_create(nullptr, 1, 2, 0, 0);
	assert(client);

	ENetAddress address;
	enet_address_set_host(&address, "localhost");
	address.port = 5123;

	ENetPeer* peer = enet_host_connect(client, &address, 2, 0);
	assert(peer);

	int event_status = 1;
	ENetEvent event;

	SDL_Log("Waiting up to 5 seconds for connection...\n");
	event_status = enet_host_service(client, &event, 5000);
}

void OnlineSpaceGame::Update(float deltatime)
{
	int event_status = 1;
	ENetEvent event;
	event_status = enet_host_service(client, &event, 0);

	while (event_status > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			SDL_Log("Connected to %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			SDL_Log("Message from server (%s:%d)\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);

			SetState(GetData(event.packet));

			enet_packet_destroy(event.packet);
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			SDL_Log("Disconnected from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
			event.peer->data = nullptr;
			break;
		}

		event_status = enet_host_service(client, &event, 0);
	}

	SpaceGame::Update(deltatime);

	
}
