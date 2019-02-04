#include <string>
#include <regex>
#include <iostream>
//#define S "M[STA]+[SL\\*]([0-9]+)?<;>I"
#define S "PTA[TC24]([MT])?[0-9]+"
using namespace std;


int main (){

   //string s = "MTAL300<;>I";
   string s = "PTAC10";
   string se = S;
   regex e = regex (se);
   cout << "start" << endl;
   if (regex_match(s,e)){
      cout << "got a match" << endl;
   }
}
