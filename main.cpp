#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <map>
#include <unistd.h>
#include <regex>

//0 a b r 1
#define FUNC_STEP 0  //0
#define FUNC_CHAR1 2 //a
#define FUNC_CHAR2 4 //b
#define FUNC_DIRECTION 6 //r
#define FUNC_NEXT_STEP 8 // 1


using namespace std;

/*
 * instructions file exapmle
 * 0 a b r 0
 * 0 b a r 0
 *
 *
 * input example
 * abbaab
 *
 * output
 * baabba
 *
 *
 *
*/
int main(int argc, char *argv[]){

    if(argc != 3){
        cout << "Usage: <instruction file> <input_string>\n";
        return 0;
    }

    string input = argv[2];

    map<int, vector<string>> funcs;
    ifstream inputFile(argv[1]);

    if(inputFile.is_open()){
        string line;

        while(getline(inputFile, line)){
            vector<string> instructions;
            int step; //current step

            //regex to validate thee instruction
            regex rgx("(^[0-9]+ . . [lr] [0-9]+)");
            std::smatch match;

            if(line.empty()){
                continue;
            }

            step = atoi(&line[FUNC_STEP]);

            //A step can have more than one instruction
            //So we check if the map already has this step
            if(funcs.find(step) != funcs.end()){
                instructions = funcs.at(step);
            }

            instructions.push_back(line);
            funcs[step] = instructions;
        }

        if(funcs.size() > 0){
            auto it = funcs.begin();

            int step,
                index = 0;

            while(it != funcs.end()){
              //  getchar();
                //cout << "\033[2J\033[1;1H"; //clear the console

                step = it->first;
                vector<string> inst = it->second;

                bool ok = false;

                if(index == input.length()){
                    input.append("_");
                }else if(index < 0){
                    input = "_" + input;
                    index = 0;
                }

                for(string function : inst){
                    if(input[index] == function[FUNC_CHAR1] || function[FUNC_CHAR1] == '*'){
                        cout << "STEP:" << step << "  " << function << '\n';
                        input[index] = function[FUNC_CHAR2] != '*' ? function[FUNC_CHAR2] : input[index];

                        ok = true;

                        //set the direction
                        index = function[FUNC_DIRECTION] == 'r' ? ++index : --index;
                        it = funcs.find(atoi(&function[FUNC_NEXT_STEP]));

                        break;
                    }
                }

                if(!ok){
                    cout << "STEP:" << step << " THERE IS NO INSTRUCTION IN STEP '" << step << "' FOR "
                         << input[index] << "\n\n";
                    cout << input << '\n';
                    return 0;
                }

                cout << input << "\n\n";
            }
        }

    }else{
        cout << "Failed to open \"" << argv[1] << "\"\n";
    }
    return 0;
}
