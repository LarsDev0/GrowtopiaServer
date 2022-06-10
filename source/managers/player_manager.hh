#include <iostream>

class PlayerManager{
    private:
        std::vector<Player*> players;
    public:
        size_t get_player_count(){ return this->players.size(); }

        void add_player(Player* player){
            for (int i = 0; i < this->players.size(); i++){
                if (this->players[i]->get_address() == player->get_address()){
                    return;
                }
            }

            this->players.push_back(player);
        }

        void remove_player(Player* player){
            for (int i = 0; i < this->players.size(); i++){
                if (this->players[i]->get_address() == player->get_address()){
                    this->players.erase(this->players.begin() + i);
                }
            }
        }

        Player* fetch_player(ENetPeer* peer){
            std::vector<char> ip;
            std::string address = "";
            ip.reserve(16);

            enet_address_get_host_ip(&peer->address, ip.data(), 16);
            address.append(ip.data()).append(":").append(std::to_string(peer->address.port));

            for (int i = 0; i < this->players.size(); i++){
                if (this->players[i]->get_address() == address){
                    return this->players[i];
                }
            }

            return nullptr;
        }
};