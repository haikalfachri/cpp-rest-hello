#include "user_controller.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>

#include "../../globals/data_types/user_data_type.h"
#include "../../globals/globals.h"

using namespace std;

UserController::UserController() {}

void UserController::get_all_users(const crow::request &kReq, crow::response &res) {
    try {
        pqxx::work txn{database->get_connection()};
        pqxx::result result = txn.exec("SELECT * FROM users");
        txn.commit();

        vector<User> users;

        for (auto row : result) {
            User user;
            user.id = row[0].as<int>();
            user.name = row[1].as<string>();
            user.created_at = row[2].as<string>();
            user.updated_at = row[3].as<string>();
            users.push_back(user);
        }

        rapidjson::StringBuffer buffer(0, 1000);
        rapidjson::Writer<rapidjson::StringBuffer> response(buffer);
        response.StartObject();
        response.String("data", 4);
        response.StartArray();
        for (User user : users) {
            response.StartObject();
            response.String("id", 2);
            response.Int(user.id);
            response.String("name", 4);
            response.String(user.name.c_str());
            response.String("created_at", 10);
            response.String(user.created_at.c_str());
            response.String("updated_at", 10);
            response.String(user.updated_at.c_str());
            response.EndObject();
        }
        response.EndArray();
        response.String("message", 7);
        response.String("Users fetched successfully");
        response.EndObject();
        res.add_header("content-type", "application/json");
        res.write(buffer.GetString());
        res.end();
    } catch (std::exception const &err) {
        std::cerr << "Exception: " << err.what() << std::endl;
        res.code = 500;
        rapidjson::StringBuffer buffer(0, 1000);
        rapidjson::Writer<rapidjson::StringBuffer> response(buffer);
        response.StartObject();
        response.String("message", 7);
        response.String("An error occurred while fetching all users");
        response.String("error", 5);
        response.String(err.what());
        response.EndObject();
        res.add_header("content-type", "application/json");
        res.write(buffer.GetString());
        res.end();
    }
}

void UserController::create_user(const crow::request &kReq, crow::response &res) {
    try {
        rapidjson::Document request;
        request.Parse(kReq.body.c_str());

        User user;
        user.name = request["name"].GetString();

        if (user.name.length() > 255 || user.name.length() < 1) {
            res.code = 400;
            rapidjson::StringBuffer buffer(0, 1000);
            rapidjson::Writer<rapidjson::StringBuffer> response(buffer);
            response.StartObject();
            response.String("message", 7);
            response.String("An error occurred while creating the user");
            response.String("error", 5);
            response.String("Name must be between 1 and 255 characters");
            response.EndObject();
            res.add_header("content-type", "application/json");
            res.write(buffer.GetString());
            res.end();
            return;
        }

        pqxx::work txn{database->get_connection()};

        pqxx::result result = txn.exec_params(
            "INSERT INTO users (name, created_at, updated_at) VALUES ($1, NOW(), NOW()) RETURNING "
            "id, name, created_at, updated_at",
            user.name);

        txn.commit();

        cout << "User created successfully" << endl;

        User created_user;
        for (auto row : result) {
            cout << "ID = " << row[0].as<int>() << endl;
            created_user.id = row[0].as<int>();
            created_user.name = row[1].as<string>();
            created_user.created_at = row[2].as<string>();
            created_user.updated_at = row[3].as<string>();
        }

        rapidjson::StringBuffer buffer(0, 1000);
        rapidjson::Writer<rapidjson::StringBuffer> response(buffer);
        response.StartObject();
        response.String("method", 6);
        response.String("POST");
        response.String("data", 4);
        response.StartObject();
        response.String("id", 2);
        response.Int(created_user.id);
        response.String("name", 4);
        response.String(created_user.name.c_str());
        response.String("created_at", 10);
        response.String(created_user.created_at.c_str());
        response.String("updated_at", 10);
        response.String(created_user.updated_at.c_str());
        response.EndObject();
        response.String("message", 7);
        response.String("User created successfully");
        response.EndObject();
        res.add_header("content-type", "application/json");
        res.write(buffer.GetString());
        res.code = 201;
        res.end();

        std::string json_response = buffer.GetString();
        cout << "Publishing message to channel: " << json_response << endl;
        publisher->publish_message(json_response);
    } catch (std::exception const &err) {
        std::cerr << "Exception: " << err.what() << std::endl;
        res.code = 500;
        rapidjson::StringBuffer buffer(0, 1000);
        rapidjson::Writer<rapidjson::StringBuffer> response(buffer);
        response.StartObject();
        response.String("message", 7);
        response.String("An error occurred while creating the user");
        response.String("error", 5);
        response.String(err.what());
        response.EndObject();
        res.add_header("content-type", "application/json");
        res.write(buffer.GetString());
        res.end();
    }
}
