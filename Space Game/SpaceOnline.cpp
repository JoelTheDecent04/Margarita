#include "SpaceOnline.h"
#include <sstream>
#undef max

#include <algorithm>

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
				entity->fx(),
				entity->fy()
			);
		}
	}
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
		if (item.nCount > 1)
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
}
void SpaceOnline::Update(float deltatime)
{
	ENetEvent event;
	int event_status = enet_host_service(client, &event, 0);
	{
		if (event_status > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				SDL_Log("New connection from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
			{
				SDL_Log("Message from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
				/*for (int i = 0; i < event.packet->dataLength; i++)
					SDL_Log("%c", event.packet->data[i]);
				SDL_Log("\n");*/

				last_packet = flatbuffers::GetRoot<ServerPacket>((char*)event.packet->data);
				nWave = last_packet->wave_number();
				fSecondsUntilNextWave = last_packet->time_left();


				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
				SDL_Log("Disconnected from %s:%d\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
				event.peer->data = nullptr;
				break;
			}
		}
	}
}
void SpaceOnline::LeftClick()
{

}
void SpaceOnline::KeyDown(int key)
{

}
SpaceOnline::SpaceOnline(const std::string& ip)
{
	int ret = enet_initialize();
	assert(ret == 0);

	client = enet_host_create(nullptr, 1, 2, 0, 0);
	
	ENetAddress address;
	enet_address_set_host(&address, ip.c_str());
	address.port = 5123;

	server_peer = enet_host_connect(client, &address, 2, 0);
	assert(server_peer);

	int event_status = 1;
	ENetEvent event;

	SDL_Log("Waiting up to 5 seconds for connection\n");
	event_status = enet_host_service(client, &event, 5000);
	if (event_status != ENET_EVENT_TYPE_CONNECT)
		SDL_Log("Failed to connect\n");
	else
		SDL_Log("Connected to %s:%u\n", ip_representation(event.peer->address.host).c_str(), event.peer->address.port);
	
	nPlayerID = -1;
	player.fX = 100;
	player.fY = 100;
	player.fSpeedX = 0;
	player.fSpeedY = 0;
	player.fHealth = -1;
	player.fMaxHealth = -1;
	player.fEnergy = -1;
	player.fMaxEnergy = -1;
	player.fMoney = 0;

	nCurrentItem = 0;
	player.vItems.push_back({ TextureID::None, -1, "None" });

	fSecondsUntilNextWave = -1.0f;
	nWave = -1;

	last_packet = nullptr;
}
SpaceOnline::~SpaceOnline()
{
	enet_deinitialize();
}