//
// Created by nejcsorgo on 1/10/2026.
//

#include "request.h"
int request::login(std::string username, std::string password) {
    if (!handle)
        return -1;
    std::string request_url = API_URL + "/login";
    std::string json_string = R"({"username":")" + username + R"(,"password":")" + password + R"("})";
    struct curl_slist *slist1;
    slist1 = nullptr;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    std::string response;

    curl_easy_setopt(handle,CURLOPT_URL,request_url.c_str());
    curl_easy_setopt(handle, CURLOPT_URL, "https://example.com/");
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &response);
    CURLcode res = curl_easy_perform(handle);
    if (res == CURLE_OK) {
        std::size_t found = response.find("\"token\":");
        if (found != std::string::npos) {
            std::size_t end = response.find('\"',found + 11);
            if (end != std::string::npos)
                token = response.substr(found+9,end - found + 9);
            else return -1;
        } else return -1;
    } else
        return res;
    std::cout<<"Uspešna prijava."<<std::endl;
    return 0;
}

inline Blockchain request::getBlockchain() {
    std::string request_url = API_URL + "/blockchain";
    std::string response;
    if (!handle || token.empty())
        throw std::invalid_argument("fali handle ali token");
    curl_slist *slist1;
    slist1 = nullptr;
    std::string t_string = "Authorization: Bearer " + token;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    slist1 = curl_slist_append(slist1,t_string.c_str() );
    curl_easy_setopt(handle,CURLOPT_URL,request_url.c_str());
    curl_easy_setopt(handle, CURLOPT_URL, "https://example.com/");
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);

    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &response);

    CURLcode res = curl_easy_perform(handle);
}

int request::remove(int index) {
}

int request::post_block(Block block) {
    
    std::string request_url = API_URL + "/blockchain";
    std::string response;
    if (!handle || token.empty())
        throw std::invalid_argument("fali handle ali token");
    curl_slist *slist1;
    slist1 = nullptr;
    std::string t_string = "Authorization: Bearer " + token;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    slist1 = curl_slist_append(slist1,t_string.c_str() );
    curl_easy_setopt(handle,CURLOPT_URL,request_url.c_str());
    curl_easy_setopt(handle, CURLOPT_URL, "https://example.com/");
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);

    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &response);

    CURLcode res = curl_easy_perform(handle);

}

Blockchain request::getBlock() {
}
