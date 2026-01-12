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
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &response);
    CURLcode res = curl_easy_perform(handle);
    //glupi json parsing
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

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
//------AI CODE-------
Blockchain request::getBlockchain() {
    std::string request_url = API_URL + "/blockchain";
    std::string response;

    if (!handle || token.empty())
        throw std::invalid_argument("fali handle ali token (Missing handle or token)");

    // --- cURL Setup ---
    struct curl_slist *slist1 = nullptr;
    std::string t_string = "Authorization: Bearer " + token;

    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    slist1 = curl_slist_append(slist1, t_string.c_str());

    curl_easy_setopt(handle, CURLOPT_URL, request_url.c_str());

    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "GET"); // Changed to GET (standard for fetching data), change back to POST if your API requires it.
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);

    // FIX: Set the callback and the string pointer
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(handle);

    // Clean up headers
    curl_slist_free_all(slist1);

    if (res != CURLE_OK) {
        throw std::runtime_error("cURL failed: " + std::string(curl_easy_strerror(res)));
    }

    // --- JSON Conversion to Existing String Format ---
    try {
        using json = nlohmann::json;
        json json_chain = json::parse(response);

        // We need to build a string like:
        // "1;prev;123;data;4;123;hash,2;prev;123;data..."
        std::string serialized_chain = "";

        // Iterate over the JSON array
        for (size_t i = 0; i < json_chain.size(); i++) {
            auto& block = json_chain[i];

            // Reconstruct the specific string format expected by Block::fromString
            // Order: index;previousHash;timestamp;data;difficulty;token;hash
            std::string block_str =
                    std::to_string(block["index"].get<int>()) + ";" +
                    block["previousHash"].get<std::string>() + ";" +
                    std::to_string(block["timestamp"].get<long>()) + ";" +
                    block["data"].get<std::string>() + ";" +
                    std::to_string(block["difficulty"].get<int>()) + ";" +
                    std::to_string(block["token"].get<int>()) + ";" +
                    block["hash"].get<std::string>();

            serialized_chain += block_str;

            // Add comma delimiter if not the last block (expected by Blockchain::fromString)
            if (i + 1 != json_chain.size()) {
                serialized_chain += ",";
            }
        }

        // --- Use Existing Method ---
        // Now that we have the string in the format your class likes, we use the existing method
        return Blockchain::fromString(serialized_chain);

    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON or converting to Blockchain: " << e.what() << std::endl;
        std::cerr << "Response received: " << response << std::endl;
        throw;
    }
}

int request::remove(int index) {
}

//-----AI CODE------
int request::post_block(Block block) {
    // 1. Prepare the URL
    // Based on your router: router.post("/blockchain", ...);
    std::string request_url = API_URL + "/blockchain";
    std::string response;

    if (!handle || token.empty())
        throw std::invalid_argument("fali handle ali token (Missing handle or token)");

    // 2. Prepare JSON Payload
    // We map the Block class fields to the JSON keys expected by req.body in your Node app
    nlohmann::json j;
    j["index"] = block.index;
    j["previousHash"] = block.previousHash;
    j["timestamp"] = (long)block.timestamp; // Cast time_t to long for JSON number
    j["data"] = block.data;
    j["difficulty"] = block.difficulty;
    j["token"] = block.token;
    j["hash"] = block.hash;

    std::string json_payload = j.dump();

    // 3. Setup Headers
    struct curl_slist *slist1 = nullptr;
    std::string t_string = "Authorization: Bearer " + token;

    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    slist1 = curl_slist_append(slist1, t_string.c_str());

    // 4. Setup cURL Options
    curl_easy_setopt(handle, CURLOPT_URL, request_url.c_str());

    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);

    // Send the JSON string as the body
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, json_payload.c_str());

    // Fix: Use callback for response
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);

    // 5. Perform Request
    CURLcode res = curl_easy_perform(handle);

    // 6. Get HTTP Status Code
    long http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);

    // Cleanup headers
    curl_slist_free_all(slist1);

    if (res != CURLE_OK) {
        std::cerr << "cURL failed: " << curl_easy_strerror(res) << std::endl;
        return 500; // Return internal server error code on curl failure
    }

    // Optional debug output
    // std::cout << "Response: " << response << std::endl;

    return (int)http_code; // Returns 201 if successful, 500 if server error, etc.
}
Blockchain request::getBlock() {

}

std::string request::getBlockData() {
    std::string request_url = API_URL + "/hiveWeight/getAllByDate";
    std::string response;

    if (!handle || token.empty())
        throw std::invalid_argument("fali handle ali token (Missing handle or token)");

    // 1. Get Current Date in "YYYY-MM-DD" format
    // The API expects: { "time_weight": "2025-09-30" }
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    std::stringstream ss;
    ss << std::put_time(now, "%Y-%m-%d");
    std::string current_date = ss.str();

    // 2. Create JSON Payload
    nlohmann::json j;
    j["time_weight"] = current_date;
    std::string json_payload = j.dump();

    // 3. Setup Headers
    struct curl_slist *headers = nullptr;
    std::string auth_header = "Authorization: Bearer " + token;

    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, auth_header.c_str());

    // 4. Setup cURL
    curl_easy_setopt(handle, CURLOPT_URL, request_url.c_str());
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST"); // API is router.post
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

    // Clean up
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
        std::cerr << "cURL failed: " << curl_easy_strerror(res) << std::endl;
        return ""; // Return empty string on network failure
    }

    if (http_code != 200) {
        std::cerr << "API Error: Received HTTP " << http_code << std::endl;
        std::cerr << "Response: " << response << std::endl;
        return ""; // Or throw exception depending on your logic
    }

    // Return the raw JSON string (e.g., "[{...}, {...}]")
    // This can be directly put into block.data
    return response;
}