// SpaceServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cassert>
#include <chrono>
#include <enet/enet.h>
#include <stdlib.h>

#include "ServerUtilities.h"
#include "OnlineSpaceGame.h"
#include "Player.h"
#include "../Margarita_generated.h"

static ENetHost* server;

void HandleEvent(const ENetEvent& event)
{
    switch (event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
        std::cout << "New connection from " << ip_representation(event.peer->address.host) << ':' << event.peer->address.port << '\n';
        event.peer->data = (void*)sgSpaceGame->AddPlayer(); //peer data (void*) is used for player id (int)
        break;
    case ENET_EVENT_TYPE_RECEIVE:
        std::cout << "Message from " << ip_representation(event.peer->address.host) << ':' << event.peer->address.port << ":\n";
        std::cout.write((const char*)event.packet->data, event.packet->dataLength);
        std::cout << '\n';

        //enet_host_broadcast(server, 0, event.packet);
        break;
    case ENET_EVENT_TYPE_DISCONNECT:
        std::cout << ip_representation(event.peer->address.host) << ':' << event.peer->address.port << " disconnected\n";
        event.peer->data = nullptr;
        break;
    }
}

int main(int argc, char** argv)
{
    std::cout << "Starting Margarita Server...\n";
    
    enet_initialize();

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 5123;

    server = enet_host_create(&address, 32, 2, 0, 0);
    assert(server);

    std::cout << "Server open on " << ip_representation(server->address.host) << ':' << server->address.port << '\n';

    sgSpaceGame = std::make_shared<OnlineSpaceGame>();

    

    int event_status;
    ENetEvent event = { };

    while (true) //Wait for players to connect
    {
        std::cout << "Waiting up to 20 seconds for connection\n";
        event_status = enet_host_service(server, &event, 20000);
        if (event.type != ENET_EVENT_TYPE_NONE)
            HandleEvent(event);

        std::cout << "Wait for more players? (Y/N) ";

        char c = getchar();
        if (c == 'y' || c == 'Y')
            continue;
        else
            break;
    }


    auto start_time = std::chrono::high_resolution_clock::now();
    
    while (true) //Main game server loop
    {
        event_status = enet_host_service(server, &event, 0);
        {
            if (event_status > 0)
                HandleEvent(event);
        }

        auto current_time = std::chrono::high_resolution_clock::now();
        auto delta_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
        start_time = current_time;

        bool game_running = sgSpaceGame->Update(delta_time.count());
        if (false/*game_running == false*/)
        {
            std::cout << "Game ended. Exiting\n";
            break;
        }
        else
        {
            flatbuffers::FlatBufferBuilder builder(2048);
                                  
            

            //Entity data
            std::vector<flatbuffers::Offset<NetEntity>> entity_data_vector;
            std::vector<flatbuffers::Offset<NetPlayer>> player_data_vector;

            for (auto& entity : sgSpaceGame->vEntities)
            {
                auto entity_data = entity->Serialise(builder);
                entity_data_vector.push_back(entity_data);
            }

            for (auto& player : sgSpaceGame->vPlayers)
            {
                auto player_data = player->SerialisePlayer(builder);
                player_data_vector.push_back(player_data);
            }

            auto entity_vector = builder.CreateVector(entity_data_vector);
            auto player_vector = builder.CreateVector(player_data_vector);

            auto server_packet = CreateServerPacket(
                builder,
                sgSpaceGame->fSecondsUntilNextWave,
                sgSpaceGame->nWave,
                player_vector,
                entity_vector
            );

            builder.Finish(server_packet);
            
            uint8_t* server_packet_buffer = builder.GetBufferPointer();
            size_t server_packet_size = builder.GetSize();

            ENetPacket* packet = enet_packet_create(server_packet_buffer, server_packet_size, ENET_PACKET_FLAG_RELIABLE /*| ENET_PACKET_FLAG_NO_ALLOCATE*/);
            enet_host_broadcast(server, 0, packet);
            std::cout << "Sent message\n, time left: " << sgSpaceGame->fSecondsUntilNextWave << '\n';
            //enet_packet_destroy(packet);

            Sleep(5);
            
        }
    }

    enet_deinitialize();
}