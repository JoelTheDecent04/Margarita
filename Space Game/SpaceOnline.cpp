#include "SpaceOnline.h"

#ifdef SUPPORT_MULTIPLAYER
#include <sstream>
#undef max

#include <algorithm>
#include "Player.h"
#include "Utilities.h"
#include "ControlsScreen.h"
#include "TitleScreen.h"
#include "JoinServerScreen.h"

std::string ip_representation(uint32_t ip)
{
	std::stringstream ss;

	ss << (ip & 0xff) << '.' << ((ip & 0xff00) >> 8) << '.' << ((ip & 0xff0000) >> 16) << '.' << ((ip & 0xff000000) >> 24);

	return ss.str();
}

void SpaceOnline::Render()
{
	fBackgroundPosition = player.fX - (1280 / 2);
	if (player.fX < (1280 / 2))
		fBackgroundPosition = 0;
	if (player.fX > 5120 - (1280 / 2))
		fBackgroundPosition = 5120 - 1280;

	textures[TextureID::Background]->DrawPanorama(fBackgroundPosition);

	if (last_packet)
	{
		auto* entity_vector = last_packet->entities();
		for (const auto& entity : *entity_vector)
		{
			textures[entity->texture()]->Draw(
				entity->frame(),
				entity->fx() - fBackgroundPosition,
				entity->fy(),
				false, //bRealCoordinates = false
				entity->rotation()
			);
		}

		auto* player_vector = last_packet->players();

		for (const auto& player : *player_vector)
		{
			if (player->id() != nPlayerID && player->alive() == true)
				textures[player->texture()]->Draw(
					player->frame(),
					player->fx() - fBackgroundPosition,
					player->fy()
				);
		}
	}

	//Render local player
	if (player.alive)
		textures[TextureID::Character]->Draw(
			0, 
			player.fX - (textures[TextureID::Character]->fTextureDrawnWidth / 2.0f) - fBackgroundPosition, 
			player.fY - textures[TextureID::Character]->fTextureDrawnHeight / 2.0f
			);

	/*for (auto& entity : vEntities)
		entity.Draw();*/

	textures[TextureID::Foreground]->Draw(0, -fBackgroundPosition - 65.0f, 475.0f);

	char txtBuf[128];

	snprintf(txtBuf, sizeof(txtBuf), "HP %d / %d", (int)player.fHealth, (int)player.fMaxHealth);
	Graphics::WriteText(txtBuf, 5, 2, Graphics::pFont14); //2
	Graphics::FillRectangle(5, 2 + 14 + 4, 100.0f * (player.fHealth / player.fMaxHealth), 20, clrRed); //18
	Graphics::DrawRectangle(5, 2 + 14 + 4, 100, 20, clrDarkGrey);

	snprintf(txtBuf, sizeof(txtBuf), "Energy %d / %d", (int)player.fEnergy, (int)player.fMaxEnergy);
	Graphics::WriteText(txtBuf, 5, 2 + 14 + 2 + 20 + 2, Graphics::pFont14); //40
	Graphics::FillRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100.0f * (player.fEnergy / player.fMaxEnergy), 20, clrBlue); //56
	Graphics::DrawRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100, 20, clrDarkGrey);

	TextSize textsize;
	Graphics::TextMetrics(player.vItems[nCurrentItem].name.c_str(), Graphics::pFont14, textsize);
	Graphics::FillRectangle(0, nScreenHeight - 4 - 32 - 4 - 14, std::max(32 * player.vItems.size(), (unsigned)textsize.width) + 10, 14 + 4 + 32 + 4, clrBlack);
	Graphics::WriteText(player.vItems[nCurrentItem].name.c_str(), 5, nScreenHeight - 4 - 32 - 4 - 14, Graphics::pFont14);

	int nItem = 0;
	for (auto& item : player.vItems)
	{
		textures[item.nTexture]->Draw(0, 4 + nItem * 32, nScreenHeight - 4 - 32, true);
		Graphics::DrawRectangle(4 + nItem * 32, nScreenHeight - 4 - 32, 32, 32, nItem == nCurrentItem ? clrWhite : clrDarkGrey);
		if (item.nCount >= 1)
		{
			snprintf(txtBuf, sizeof(txtBuf), "%d", item.nCount);
			Graphics::WriteText(txtBuf, 6 + nItem * 32, nScreenHeight - 4 - 16, Graphics::pFont14);
		}
		nItem++;
	}

	snprintf(txtBuf, 64, "$%d", (int)player.fMoney);
	Graphics::TextMetrics(txtBuf, Graphics::pFont16, textsize);
	Graphics::WriteText(txtBuf, nScreenWidth - 5 - textsize.width, 5, Graphics::pFont16);

	snprintf(txtBuf, 64, "Wave %d (%d seconds left)", nWave, (int)fSecondsUntilNextWave);
	Graphics::TextMetrics(txtBuf, Graphics::pFont16, textsize);
	Graphics::FillRectangle(nScreenWidth - 5 - textsize.width - 5, nScreenHeight - textsize.height, 5 + textsize.width + 3, textsize.height, clrBlack);
	Graphics::WriteText(txtBuf, nScreenWidth - 5 - textsize.width, nScreenHeight - textsize.height, Graphics::pFont16);

	if (!bGameStarted)
	{
		const char* status_text = bDidConnect ? "Waiting for Game to Start" : "Failed to Connect :(";
		Graphics::TextMetrics(status_text, Graphics::pFont24, textsize);
		Graphics::WriteText(status_text, nScreenWidth / 2 - textsize.width / 2, 10, Graphics::pFont24);
	}

	if (last_packet && last_packet->wave_finished())
	{
		if (!player.ready)
			snprintf(txtBuf, 64, "Wave Completed. Press '%s' To Continue.", ControlsScreen::KeyText(keyNextWave1));
		else
		{
			int nTotalPlayers = 0;
			int nReadyPlayers = 0;
			for (int i = 0; i < last_packet->players()->size(); i++)
			{
				const auto& player = last_packet->players()->Get(i);
				if (player->alive()) nTotalPlayers++;
				if (player->alive() && player->ready()) nReadyPlayers++;
			}
			
			int nNeededPlayers = (int)ceil(nTotalPlayers / 2.0f);

			snprintf(txtBuf, 64, "Wave Completed. Waiting For %d More Players", nNeededPlayers - nReadyPlayers);
		}
		Graphics::TextMetrics(txtBuf, Graphics::pFont24, textsize);
		Graphics::WriteText(txtBuf, nScreenWidth / 2 - textsize.width / 2, 8, Graphics::pFont24);
	}

	if (server_peer)
	{
		snprintf(txtBuf, 64, "Ping - %dms (%.1f%% packet loss)", server_peer->lastRoundTripTime, (float)(server_peer->packetLoss / ENET_PEER_PACKET_LOSS_SCALE));
		Graphics::TextMetrics(txtBuf, Graphics::pFont12, textsize);
		Graphics::WriteText(txtBuf, nScreenWidth / 2 - textsize.width / 2, nScreenHeight - 4 - textsize.height, Graphics::pFont12);
	}
}
void SpaceOnline::Update(float deltatime)
{
	ENetEvent event;
	while (enet_host_service(client, &event, 0) > 0)
	{		
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			SDL_Log("New connection from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
			break;
		case ENET_EVENT_TYPE_RECEIVE:
		{
			bGameStarted = true;
			//SDL_Log("Message from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);

			switch (event.channelID)
			{
			case 0: //Regular data update
			{
				if (last_enet_packet)
					enet_packet_destroy(last_enet_packet);
				last_enet_packet = event.packet;
				last_packet = flatbuffers::GetRoot<ServerPacket>((char*)event.packet->data);
				if (nWave != last_packet->wave_number())
				{ //New wave has started
					player.ready = false; //Reset 'ready' status
				}
				nWave = last_packet->wave_number();
				fSecondsUntilNextWave = last_packet->time_left();

				const auto& player_data = last_packet->players()->Get(nPlayerID);
				player.fEnergy = player_data->energy();
				player.fHealth = player_data->health();
				break;
			}

			case 1: //Player ID assignment
				nPlayerID = *(uint32_t*)event.packet->data;
				break;

			case 2: //Background Object

				SDL_Log("Background object\n");
				break;

			default:
				abort();
			}

			//SDL_Log("Messaged received (%f seconds left)\n", fSecondsUntilNextWave);
			break;
		}
		case ENET_EVENT_TYPE_DISCONNECT:
			SDL_Log("Disconnected from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
			event.peer->data = nullptr;
			Game::LoadLevel(std::make_shared<JoinServerScreen>());
			return;
		}
	}

	//Local player movements
	if (KeyState(keyMoveRight1) || KeyState(keyMoveRight2))
	{
		player.fSpeedX += fPlayerAcceleration * deltatime;
		if (player.fSpeedX > player.fMovementSpeed)
			player.fSpeedX = player.fMovementSpeed;
	}
	if (KeyState(keyMoveLeft1) || KeyState(keyMoveLeft2))
	{
		player.fSpeedX -= fPlayerAcceleration * deltatime;
		if (player.fSpeedX < -player.fMovementSpeed)
			player.fSpeedX = -player.fMovementSpeed;
	}
	if (KeyState(keyMoveDown1) || KeyState(keyMoveDown2))
		player.fSpeedY += fPlayerMoveDownSpeed * deltatime;

	if (KeyState(keyJump1) || KeyState(keyJump2))
	{
		player.fSpeedY = -180.0f;
		player.bOnGround = false;
	}

	if (player.fSpeedX > 0)
	{
		player.fSpeedX -= fPlayerDecceleration * deltatime;
		if (player.fSpeedX < 0)
			player.fSpeedX = 0;
	}
	if (player.fSpeedX < 0)
	{
		player.fSpeedX += fPlayerDecceleration * deltatime;
		if (player.fSpeedX > 0)
			player.fSpeedX = 0;
	}
	if (!player.bOnGround)
		player.fSpeedY += fGravity * deltatime;

	player.fX += player.fSpeedX * deltatime;
	player.fY += player.fSpeedY * deltatime;

	if (player.fX < 0)
		player.fX = 0;
	if (player.fX > 5120)
		player.fX = 5120;
	if (player.fY <= 0.0f)
	{
		player.fY = 0.0f;
		player.fSpeedY = 0.0f;
	}
	if (player.fY >= 594.0f - 58 / 2) //58 = player height
	{
		player.fY = 594.0f - 58 / 2;
		player.fSpeedY = 0.0f;
		player.bOnGround = true;
	}

	//SDL_Log("Client Y position: %f\n", player.fY);

	//Send player data to server
	flatbuffers::FlatBufferBuilder builder(1024);
	auto client_packet = CreateClientPacket(builder, bFiredWeapon ? weapon_fire.weapon : -1, weapon_fire.angle, weapon_fire.intensity, false, 
											player.ready, player.fX, player.fY, player.fMaxHealth, player.fMaxEnergy);
	builder.Finish(client_packet);

	uint8_t* client_packet_buffer = builder.GetBufferPointer();
	size_t client_packet_size = builder.GetSize();

	ENetPacket* packet = enet_packet_create(client_packet_buffer, client_packet_size, bFiredWeapon ? ENET_PACKET_FLAG_RELIABLE : 0 /*| ENET_PACKET_FLAG_NO_ALLOCATE*/);
	enet_peer_send(server_peer, 0, packet);
	enet_host_flush(client);

	bFiredWeapon = false;
}
void SpaceOnline::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);

	nCursorX += fBackgroundPosition;

	float fPlayerCentreX = player.fX; //+textures[TextureID::Character]->fTextureDrawnWidth / 2.0f;
	float fPlayerCentreY = player.fY; //+textures[TextureID::Character]->fTextureDrawnHeight / 2.0f;

	float fGradient = (nCursorY - fPlayerCentreY) / (nCursorX - fPlayerCentreX);
	float fAngle = atan(fGradient);
	if (nCursorX < fPlayerCentreX) fAngle += 3.1415926f;

	weapon_fire.angle = fAngle;
	weapon_fire.weapon = player.vItems[nCurrentItem].ID;
	weapon_fire.intensity = player.vItems[nCurrentItem].nCount;
	bFiredWeapon = true;
}
void SpaceOnline::KeyDown(int key)
{
	if ((key == keyNextWave1 || key == keyNextWave2) && last_packet && last_packet->wave_finished())
		player.ready = true;
	if (key == SDL_SCANCODE_ESCAPE)
		Game::LoadLevel(std::make_shared<TitleScreen>());

	for (int i = 0; i < 9; i++)
	{
		if (key == keyChangeWeapon1[i] || key == keyChangeWeapon2[i])
			if (player.vItems.size() >= i + 1)
				nCurrentItem = i;
	}
}
SpaceOnline::SpaceOnline(const std::string& ip)
{
	int ret = enet_initialize();
	assert(ret == 0);

	client = enet_host_create(nullptr, 1, 4, 0, 0);
	
	ENetAddress address;
	enet_address_set_host(&address, ip.c_str());
	address.port = 5123;

	server_peer = enet_host_connect(client, &address, 4, 0);
	assert(server_peer);

	int event_status = 1;
	ENetEvent event;

	SDL_Log("Waiting up to 5 seconds for connection\n");
	event_status = enet_host_service(client, &event, 5000);
	if (event_status != ENET_EVENT_TYPE_CONNECT)
	{
		SDL_Log("Failed to connect\n");
		bDidConnect = false;
	}
	else
	{
		SDL_Log("Connected to %s:%u\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
		bDidConnect = true;
	}
	
	nPlayerID = -1;
	player.fX = 100;
	player.fY = 100;
	player.fSpeedX = 0;
	player.fSpeedY = 0;
	player.fHealth = -1;
	player.fMaxHealth = 200;
	player.fEnergy = -1;
	player.fMaxEnergy = 150;
	player.fMoney = 0;
	player.bOnGround = false;
	player.fMovementSpeed = 150.0f;
	player.alive = true;
	player.ready = false;

	nCurrentItem = 0;
	player.vItems.push_back({ 1, TextureID::Laser, 1, "Laser" }); //1 = laser
	player.vItems.push_back({ 2, TextureID::Orb, 0, "Orb" }); //2 = Orb

	fSecondsUntilNextWave = -1.0f;
	nWave = -1;

	bFiredWeapon = false;

	last_packet = nullptr;
	last_enet_packet = nullptr;
	bGameStarted = false;
}
SpaceOnline::~SpaceOnline()
{
	if (client)
		enet_host_destroy(client);
	enet_deinitialize();
}

#endif