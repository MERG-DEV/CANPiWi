#ifndef __OPC_CODES_H
#define __OPC_CODES_H


#include <string>
#include <map>
#include "opcodes.h"

using namespace std;

/*
* Storage class for opc codes
*/
class opc_code{    
    public:     
        opc_code() = default;   
        opc_code(int code, string name, string description){
            this->code = code;
            this->name = name;
            this->description = description;
        };
        int getCode(){ return code; };
        string getName() { return name; };
        string getDescription() { return description;};
    private:
        int code;
        string name;
        string description;
};

/*
* Class that allows search of opcs
*/
class opc_container{    
    public:
        
        opc_container();
        opc_code *getByCode(int code);
        opc_code *getByName(string name);
    private:
        map <int, opc_code*> opcs_by_code;
        map <string, opc_code*> opcs_by_name;        
        void populate();  
        void populate_both(int code, string name, string description);
};

#endif