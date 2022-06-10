#include <iostream>

#include "objects/player.hh"
#include "objects/event.hh"

#include "managers/player_manager.hh"

#include <enet/enet.h>

class Server{
    private:
        ENetAddress address;
        ENetHost* server;
        ENetEvent event;

        PlayerManager* player_manager = new PlayerManager();

    public:
        bool run(const char* host, int port){
            if (enet_initialize() != 0) {
                return false;
            }

            if (enet_address_set_host(&this->address, host) != 0) { return false; }

            this->address.port = port;

            this->server = enet_host_create(&this->address, 500, 2, 2024, 2024);
            this->server->checksum = enet_crc32;

            enet_host_compress_with_range_coder(this->server);

            if (!this->server){
                return false;
            }

            atexit(enet_deinitialize);

            while (true) {
                enet_host_service(this->server, &this->event, 10);

                switch (this->event.type) {
                    case ENET_EVENT_TYPE_CONNECT:{
                        Player* p = new Player();
                        p->set_peer(this->event.peer);

                        this->player_manager->add_player(p);
                        p->send_packet(1);
                        std::cout << p->get_address() << " connected" << std::endl;

                        break;
                    }
                    case ENET_EVENT_TYPE_DISCONNECT:{
                        Player* player = this->player_manager->fetch_player(this->event.peer);

                        if (!player){ break; }

                        this->player_manager->remove_player(player);
                        std::cout << "disconnected -> " << player->get_address() << std::endl;
                        
                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE:{
                        Player* player = this->player_manager->fetch_player(this->event.peer);

                        if (!player){ break; }

                        uint32_t packet_type = static_cast<uint32_t>(*this->event.packet->data); //these 2 lines were scammed from beef's src!!!!!! ezzzzzzzzz
                        std::string packet = reinterpret_cast<char*>(this->event.packet->data + 4);

                        std::cout << "Packet type: " << packet_type << std::endl;

                        switch (packet_type){
                            case 2: //text
                                if (packet.find("requestedName") != std::string::npos){
                                    std::cout << "login request" << std::endl;
                                    player->send_console_message("test123");
                                }

                                break;
                            case 3: //action
                                if (packet == "action|quit"){
                                    player->disconnect();
                                    player_manager->remove_player(player);
                                }

                                break;
                        }

                        enet_packet_destroy(this->event.packet);

                        break;
                    }
                }
            }
        }
};