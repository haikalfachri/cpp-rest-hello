#pragma pack(0)

#include <string>

#ifndef USERDATATYPE_H
#define USERDATATYPE_H

struct User {
    int id;
    std::string name;
    std::string created_at;
    std::string updated_at;
};

#endif