#include <iostream>
#include <string>
#include <unistd.h>
#include "mqtt/client.h"
#include <map>
#include <array>

int moved = 0;
int val;
int windowState;

std::map<std::string, int> values =
{
    {"down", -1},
    {"stop", 0},
    {"up", 1}
};

std::array<int, 4> szyby = {0,0,0,0};
std::array<int, 4> szybyFuture = {0,0,0,0};
const int  QOS = 1;

const std::string SERVER_ADDRESS	{ "tcp://localhost:1883" };
const std::string CLIENT_ID		{ "windows_control_app" };
const std::string TOPIC 			{ "/car/window/" };

void msgHandling(mqtt::const_message_ptr msg){
    int temp = std::string(msg->get_topic())[12] - 48;
    int valTemp = szybyFuture[temp];

    try{
        valTemp = values.at(std::string(msg->to_string()));
    }catch(const std::out_of_range& oor){

    }

    szybyFuture[temp] = valTemp;
}

void drawWindows(){
    std::cout<< u8"\033[2J\033[1;1H";
    for(int i = 0; i < 10; i++){
        for(int x = 0; x < 4; x++){
            if((i-szyby[x]) > 0){
                std::cout << "  |||||  ";
            }else{
                std::cout << "         ";
            }
        }
        std::cout << "\n";
    }
}


int main(){
    mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID);

	auto connOpts = mqtt::connect_options_builder().clean_session(true).finalize();

    try{
		cli.start_consuming();

		std::cout << "Connecting to the MQTT server..." << std::flush;
		auto tok = cli.connect(connOpts);
		auto rsp = tok->get_connect_response();

        for(int i = 0; i < 4; i++){
            cli.subscribe(TOPIC + std::to_string(i), QOS)->wait();
        }


        while(!false){
            mqtt::const_message_ptr msg;

            if(cli.try_consume_message(&msg)){
                if (msg == nullptr) break;
                msgHandling(msg);
            }


            drawWindows();
            std::cout.flush();
            sleep(1);
            for(int x = 0; x < 4; x++){
                val = szybyFuture[x];
                windowState = szyby[x];
                if(val != 0){
                    szyby[x] = szyby[x] - (szybyFuture[x] / abs(szybyFuture[x]));
                if(szyby[x] > 9 || szyby[x] < 0){
                    szyby[x] = windowState;
                    szybyFuture[x] = 0;
                }
            }
        }
        }


        if (cli.is_connected()){
            std::cout << "\nShutting down and disconnecting from the MQTT server..." << std::flush;
            cli.unsubscribe(TOPIC)->wait();
            cli.stop_consuming();
            cli.disconnect()->wait();
            std::cout << "OK" << std::endl;
        }else{
            std::cout << "\nClient was disconnected" << std::endl;
        }
    }catch (const mqtt::exception& exc) {
        std::cerr << "\n  " << exc << std::endl;
        return 1;
    }
    return 0;
}