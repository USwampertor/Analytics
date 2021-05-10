#include "aDatabase.h"

#define CURL_STATICLIB
#include <curl/curl.h>
#include <rapidjson/document.h>

namespace Analytics
{
  size_t
  curlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try { s->append((char*)contents, newLength); }
    catch (std::bad_alloc& e) { return 0; }
    return newLength;
  }

  void
  Database::init() {

  }

  int
  Database::post(String url, String query, String& headerStr, String& responseStr) {
    String header_string;
    String response_string = "";

    CURLcode ret;
    CURL* hnd;
    struct curl_slist* slist1;

    slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Content-Type:application/json");

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 204800L);
    curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, query.c_str());
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)query.size());
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.55.1");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

    //TODO: Change this to a lambda expression to make it easier to read
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(hnd, CURLOPT_HEADERDATA, &header_string);

    ret = curl_easy_perform(hnd);

    curl_easy_cleanup(hnd);
    hnd = nullptr;
    curl_slist_free_all(slist1);
    slist1 = nullptr;

    responseStr = response_string;
    headerStr = header_string;
    delete hnd;
    return ret;
  }

  int
  Database::get(String url,
                String query,
                String& headerStr,
                String& responseStr) {
    
    String header_string;
    String response_string = "";

    CURLcode ret;
    CURL* hnd;
    struct curl_slist* slist1;

    slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Content-Type:application/json");

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 204800L);
    curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, query.c_str());
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)query.size());
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.55.1");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

    //TODO: Change this to a lambda expression to make it easier to read
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(hnd, CURLOPT_HEADERDATA, &header_string);

    ret = curl_easy_perform(hnd);

    curl_easy_cleanup(hnd);
    hnd = nullptr;
    curl_slist_free_all(slist1);
    slist1 = nullptr;

    responseStr = response_string;
    headerStr = header_string;
    delete hnd;
    return ret;
  }



  Vector<sf::Vector3i>
    Database::downloadData() {

    Vector <sf::Vector3i> toReturn;

    String header;
    String response = "";
    String url = String("localhost:9200") + String("/") +
                 String("heatmap") + String("/_search?pretty");

    String query = "{\"query\": { \"bool\": { \"must\": [ { \"exists\": {\"field\": \"position\"} } ] } } }";
    
    get(url, query, header, response);

    if (response == "") {
      return toReturn;
    }
    
    rapidjson::Document jsonFile;
    jsonFile.Parse(response.c_str());

    if (jsonFile.IsNull()) {
      return toReturn;
    }

    if (jsonFile.HasMember("hits")) {
      if (jsonFile.HasMember("hits")) {
        if (jsonFile["hits"]["hits"].IsArray()) {
          auto arrayOfResults = jsonFile["hits"]["hits"].GetArray();
          for (rapidjson::Value::ConstValueIterator itr = arrayOfResults.Begin(); itr != arrayOfResults.End(); ++itr) {
            const rapidjson::Value& jsonObject = *itr;
            if (jsonObject.HasMember("_source")) {
              if (jsonObject["_source"].IsObject()) {
                auto sourceObject = jsonObject["_source"].GetObject();

                if (sourceObject.HasMember("position")) {
                  sf::Vector3i v;
                  
                  auto positionArray = sourceObject["position"].GetArray();
                  v.x = static_cast<int>(positionArray[0].GetFloat());
                  v.y = static_cast<int>(positionArray[1].GetFloat());
                  v.z = static_cast<int>(positionArray[2].GetFloat());
                  toReturn.push_back(v);
                }

              }
            }
          }
        }

      }
    }

    Ofstream json("result.json");
    json << response;
    json.close();

    return toReturn;
  }

}
