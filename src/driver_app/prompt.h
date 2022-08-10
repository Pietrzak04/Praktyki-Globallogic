#include <string>
#include <vector>

#define PROMPT "-> "
#define HELP_MESSAGE "autox - v0.1\n\
Commands:\n\
exit \t\t-> exit this prompt\n\
help \t\t-> write this help message\n\
open <front-left, etc> window \t\t-> open the specified window\n\
close <front-left, etc> window \t\t-> close the specified window\n\
turn_on <left or right> indicator \t\t-> turn on the specified indicator light\n\
turn_off <left or right> indicator \t\t-> turn off the specified indicator\n\
wipe <front or back> <on or once> \t\t-> turn on the specified wipers in single or continous mode\n\
wipe_w_fluid <front or back> <on or once> \t\t-> turn on the specified wipers and sprinklers in single or continous mode\n\
wipe_off <front or back> \t\t-> turn off the specified wipers\n\
"


struct command {
    std::string command;
    int len;
};

const struct command command_list[] = {
    {"exit",        1},
    {"help",        1},
    {"open",        3},
    {"close",       3},
    {"turn_on",     3},
    {"turn_off",    3},
    {"wipe",        3},
    {"wipe_w_fluid", 3},
    {"wipe_off",    2},
};


void repl_loop();
int check_operator(std::string op);
std::vector<std::string> split_line(std::string line);
bool execute_instruction(int instruction, std::vector<std::string> args);