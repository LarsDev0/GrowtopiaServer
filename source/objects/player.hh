#include <string>
#include <vector>

#include <enet/enet.h>

#include "utils/variant.hh"

class Player{
    private:
        ENetPeer* peer;
        std::vector<char> ip;
        std::string address;

        void send(ENetPacket* packet){
            enet_peer_send(this->peer, 0, packet);
        }

    public:
        std::string get_ip(){ return this->ip.data(); }
        std::string get_address(){ return this->address; }
        ENetPeer* get_peer(){ return this->peer; }

        void disconnect(){ enet_peer_reset(this->peer); } //force disconnect, otherwise server will crash :'(

        void set_peer(ENetPeer* peer){
            this->peer = peer;
            this->ip.reserve(16);

            enet_address_get_host_ip(&peer->address, this->ip.data(), 16);

            this->address = "";
            this->address.append(this->ip.data());
            this->address.append(":");
            this->address.append(std::to_string(peer->address.port));
        }

        void send_packet(int packet_type){
            std::vector<uint8_t> data(4);
            memcpy(data.data(), &packet_type, 4);

            ENetPacket* packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
            this->send(packet);
        }

        void send_console_message(std::string text){
            Variant* variant_list = new Variant();
            Value* value = new Value();
            
            value->string_value = "OnConsoleMessage";
            variant_list->append(Variant::STRING, value);

            value->string_value = text;
            variant_list->append(Variant::STRING, value);

            std::cout << variant_list->get_variant_list() << std::endl;
        }
};