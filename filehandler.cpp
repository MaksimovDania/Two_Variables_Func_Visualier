#include "filehandler.h"

using namespace std;

vector<string>* loadData(string path) //path - путь
{
    ifstream file;
    file.open(path);
    vector<string>* strings;
    try {
        strings = new vector<string>;
    } catch (exception){
        return nullptr;
    }
    string str;
    if (file){
        while (getline(file,  str)){
            strings->push_back(str);
        }
        file.close();
    }
    else{
        delete strings;
        return nullptr;
    }
    return strings;
}
