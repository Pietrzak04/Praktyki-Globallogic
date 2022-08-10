#include <cstdio>
#include <iostream>
#include <string>
#include "prompt.h"


void repl_loop()
{
    bool status = true;
    int instruction;
    std::string op, line;
    std::vector<std::string> args;

    do {
        std::cout << PROMPT;
        std::getline(std::cin, line);

        if (std::cin.eof()) {
            printf("\n");
            exit(0);
        } else if (line.length() == 0)
            continue;
        
        args = split_line(line);
        instruction = check_operator(args[0]);
        status = execute_instruction(instruction, args);
    } while (status);
}


int check_operator(std::string op)
{
    int i;
    bool found = false;

    // loop over command_list and check for op
    for (i = 0; i < (sizeof(command_list)/sizeof(command_list[0])); i++) {
        if (!command_list[i].command.compare(op)) {
            found = true;
            break;
        }
    }

    if (found)
        return i;
    else {
        printf("Syntax error: Unknown command - %s\n", op.c_str());
        return -1;
    }
}


std::vector<std::string> split_line(std::string line)
{
    std::vector<std::string> ret;

    int start = 0,
        end = line.find(" "); // delimiter between words
    
    while (end != -1) {
        ret.push_back(line.substr(start, end - start));
        start = end + 1;
        end = line.find(" ", start);
    }
    ret.push_back(line.substr(start, end - start));

    return ret;
}


bool execute_instruction(int instruction, std::vector<std::string> args)
{
    if (instruction >= 0) {
        if (args.size() > command_list[instruction].len) {
            printf("Syntax error: too many arguments - %zu\n", args.size());
            return true;
        } else if (args.size() < command_list[instruction].len) {
            printf("Syntax error: too little arguments - %zu\n", args.size());
            return true;
        }
    }

    int window = 0;
    std::string indicator = "",
        wipers = "";

    switch (instruction) {
        case -1:
            return true;

        case 0:
            exit(0);

        case 1:
            printf("%s\n", HELP_MESSAGE);
            break;

        case 2:
            if (args[2].compare("window")) {
                printf("Syntax error: unknown argument - %s\n", args[2].c_str());
                return true;
            }

            if (args[1].compare("front-left") == 0)
                window = 0;
            else if (!args[1].compare("front-right"))
                window = 1;
            else if (!args[1].compare("back-left"))
                window = 2;
            else if (!args[1].compare("back-right"))
                window = 3;
            else {
                printf("Syntax error: unknown argument - %s\n", args[1].c_str());
                return true;
            }

            printf("/car/window/%d { direction: 'down' }\n", window);
            break;

        case 3:
            if (args[2].compare("window")) {
                printf("Syntax error: unknown argument - %s\n", args[2].c_str());
                return true;
            }

            if (!args[1].compare("front-left"))
                window = 0;
            else if (!args[1].compare("front-right"))
                window = 1;
            else if (!args[1].compare("back-left"))
                window = 2;
            else if (!args[1].compare("back-right"))
                window = 3;
            else {
                printf("Syntax error: unknown argument - %s\n", args[1].c_str());
                return true;
            }

            printf("/car/window/%d { direction: 'up' }\n", window);
            break;
        
        case 4:
            if (args[2].compare("indicator")) {
                printf("Syntax error: unknown argument - %s\n", args[2].c_str());
                return true;
            }

            if (!args[1].compare("left"))
                indicator = "left";
            else if (!args[1].compare("right"))
                indicator = "right";
            else if (!args[1].compare("hazard"))
                indicator = "hazard";
            else {
                printf("Syntax error: unknown argument - %s\n", args[1].c_str());
                return true;
            }

            printf("/car/indicator/%s { status: 'on' }\n", indicator.c_str());
            break;

        case 5:
            if (args[2].compare("indicator")) {
                printf("Syntax error: unknown argument - %s\n", args[2].c_str());
                return true;
            }

            if (!args[1].compare("left"))
                indicator = "left";
            else if (!args[1].compare("right"))
                indicator = "right";
            else if (!args[1].compare("hazard"))
                indicator = "hazard";
            else {
                printf("Syntax error: unknown argument - %s\n", args[1].c_str());
                return true;
            }

            printf("/car/indicator/%s { status: 'off' }\n", indicator.c_str());
            break;

        case 6:
            if (args[1].compare("front") && args[1].compare("back")) {
                printf("Syntax error: unknown argument - %s\n", args[1].c_str());
                return true;
            }

            if (args[2].compare("on") && args[2].compare("once")) {
                printf("Syntax error: unknown argument - %s\n", args[2].c_str());
                return true;
            }

            printf("/car/wipers/%s { status: '%s' }\n", args[1].c_str(), args[2].c_str());
            break;

        case 7:
            if (args[1].compare("front") && args[1].compare("back")) {
                printf("Syntax error: unknown argument - %s\n", args[1].c_str());
                return true;
            }

            if (!args[2].compare("on"))
                wipers = "fluid";
            else if (!args[2].compare("once"))
                wipers = "fluid_once";
            else {
                printf("Syntax error: unknown argument - %s\n", args[2].c_str());
                return true;
            }

            printf("/car/wipers/%s { status: '%s' }\n", args[1].c_str(), wipers.c_str());
            break;

        case 8:
            if (args[1].compare("front") && args[1].compare("back")) {
                printf("Syntax error: unknown argument - %s\n", args[1].c_str());
                return true;
            }

            printf("/car/wipers/%s { status: 'off' }\n", args[1].c_str());
            break;

    }

    return true;
}