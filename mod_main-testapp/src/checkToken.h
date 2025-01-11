#pragma once
#include <jwt-cpp/jwt.h>
#include <httplib.h>
#include <chrono>
#include <string>
#include <iostream>
using namespace std;
inline string findToken(const httplib::Request& req)
{
    string token = "";

    if (req.has_header("Authorization")) {
        string authHeader = req.get_header_value("Authorization");

        size_t bearerPos = authHeader.find("Bearer ");
        if (bearerPos != string::npos) {
            token = authHeader.substr(bearerPos + 7);
        }

        size_t cut_pos = token.size();
        if (token.find(' ') != string::npos) {
            if (cut_pos > token.find(' '))
                cut_pos = token.find(' ');
        }
        if (token.find(' ') != string::npos) {
            if (cut_pos > token.find('\n'))
                cut_pos = token.find('\n');
        }
        if (token.find(' ') != string::npos) {
            if (cut_pos > token.find('\r'))
                cut_pos = token.find('\r');
        }

        token = token.substr(0, cut_pos);
    }

    return token;
}

inline unordered_map<jwt::traits::kazuho_picojson::string_type, jwt::claim> CheckToken(const httplib::Request& req)
{
    unordered_map<jwt::traits::kazuho_picojson::string_type, jwt::claim> payload;
    string secret = "key";

    cout << "   Find token... ";
    string token = findToken(req);

    if (token == "")
    {
        cout << "Token not found." << endl;
        return payload;
    }
    try {
        auto decoded_token = jwt::decode(token);
        auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{ secret });
        verifier.verify(decoded_token);
        payload = decoded_token.get_payload_claims();
        cout << "Valid token." << endl;
    }catch (...) {
        cout << "Invalid token." << endl;
    }

    return payload;
}
