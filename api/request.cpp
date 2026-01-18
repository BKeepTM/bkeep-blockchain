#include "request.h"

std::string request::API_URL ="https://pi.darkosever.si"; // primer
std::string request::token = ""; // tukaj se mora inicializirati, drugace problem.
CURL *request::handle = curl_easy_init();

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int request::login(std::string username, std::string password) {

    if (!handle)
        return -1;
    std::string request_url = API_URL + "/users/login";
    std::string json_string = R"({"username":")" + username + R"(","password":")" + password + R"("})";
    struct curl_slist *slist1;
    slist1 = nullptr;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    std::string response;

    curl_easy_setopt(handle,CURLOPT_URL,request_url.c_str());
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(handle);
    //glupi json parsing
    if (res == CURLE_OK) {
        std::size_t found = response.find("\"token\":");
        if (found != std::string::npos) {
            std::size_t end = response.find('\"',found + 11);
            if (end != std::string::npos)
                token = response.substr(found+9,end - found - 9);
            else return -1;
        } else return -1;
    } else
        return res;
    std::cout<<"UspeÅ¡na prijava."<<std::endl;
    return 0;
}

Blockchain request::getBlockchain() {
    std::string request_url = API_URL + "/blockchain/list";
    std::string response;

    if (!handle || token.empty())
        throw std::invalid_argument("fali handle ali token (Missing handle or token)");

    struct curl_slist *slist1 = nullptr;
    std::string t_string = "Authorization: Bearer " + token;

    //slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    slist1 = curl_slist_append(slist1, t_string.c_str());

    curl_easy_setopt(handle, CURLOPT_URL, request_url.c_str());

    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, "");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "GET"); 
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);

    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(handle);

    // Clean up headers
    curl_slist_free_all(slist1);

    if (res != CURLE_OK) {
        throw std::runtime_error("cURL failed: " + std::string(curl_easy_strerror(res)));
    }

    try {
        using json = nlohmann::json;
        json json_chain = json::parse(response);


        std::string serialized_chain = "";

        for (size_t i = 0; i < json_chain.size(); i++) {
            auto& block = json_chain[i];
            std::string block_str =
                    std::to_string(block["index"].get<int>()) + ";" +
                    block["previousHash"].get<std::string>() + ";" +
                    std::to_string(block["timestamp"].get<long>()) + ";" +
                    block["data"].get<std::string>() + ";" +
                    std::to_string(block["difficulty"].get<int>()) + ";" +
                    std::to_string(block["token"].get<int>()) + ";" +
                    block["hash"].get<std::string>();

            serialized_chain += block_str;

            if (i + 1 != json_chain.size()) {
                serialized_chain += ",";
            }
        }

        if (json_chain.empty())
            return {5,4,4};
        return Blockchain::fromString(serialized_chain);

    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON or converting to Blockchain: " << e.what() << std::endl;
        std::cerr << "Response received: " << response << std::endl;
        throw;
    }
}

int request::remove(int index) {
}

int request::post_block(Block block) {
    std::string request_url = API_URL + "/blockchain";
    std::string response;

    if (!handle || token.empty())
        throw std::invalid_argument("fali handle ali token (Missing handle or token)");

    nlohmann::json j;
    j["index"] = block.index;
    j["previousHash"] = block.previousHash;
    j["timestamp"] = (long)block.timestamp;
    j["data"] = block.data;
    j["difficulty"] = block.difficulty;
    j["token"] = block.token;
    j["hash"] = block.hash;

    std::string json_payload = j.dump();

    struct curl_slist *slist1 = nullptr;
    std::string t_string = "Authorization: Bearer " + token;

    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    slist1 = curl_slist_append(slist1, t_string.c_str());

    curl_easy_setopt(handle, CURLOPT_URL, request_url.c_str());
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, json_payload.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(handle);
    long http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);
    curl_slist_free_all(slist1);

    if (res != CURLE_OK) {
        std::cerr << "cURL failed: " << curl_easy_strerror(res) << std::endl;
        return 500;
    }
    return (int)http_code;
}
Block request::getBlock() {

}

std::string request::getBlockData() {
    std::string request_url = API_URL + "/hiveWeight/getAllByDate";
    std::string response;

    if (!handle || token.empty())
        throw std::invalid_argument("fali handle ali token (Missing handle or token)");


    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    std::stringstream ss;
    ss << std::put_time(now, "%Y-%m-%d");
    std::string current_date = ss.str();


    nlohmann::json j;
    j["time_weight"] = current_date;
    std::string json_payload = j.dump();


    struct curl_slist *headers = nullptr;
    std::string auth_header = "Authorization: Bearer " + token;

    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, auth_header.c_str());


    curl_easy_setopt(handle, CURLOPT_URL, request_url.c_str());
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST"); 
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);

    // Attach Body
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, json_payload.c_str());

    // Capture Response
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(handle);

    // Check HTTP Status Code
    long http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);

    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
        std::cerr << "cURL failed: " << curl_easy_strerror(res) << std::endl;
        return ""; 
    }

    if (http_code != 200) {
        std::cerr << "API Error: Received HTTP " << http_code << std::endl;
        std::cerr << "Response: " << response << std::endl;
        return ""; 
    }

    return response;
}