#include "main.hh"

int main() {
    Server* server = new Server();
    server->run("127.0.0.1", 10000);

    return 0;
}