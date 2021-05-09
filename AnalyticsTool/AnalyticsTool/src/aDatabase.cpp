#include "aDatabase.h"

#define CURL_STATICLIB
#include <curl/curl.h>

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



  bool
  Database::downloadData() {
   
    String header;
    String response = "";
    String url = String("localhost:9200") + String("/") +
                 String("heatmap") + String("/_search?pretty");

    String query = "{\"query\": { \"match\": {\"event_id\":\"world\"} } }";

    get(url, query, header, response);

    if (response == "") {
      return false;
    }
   
    Ofstream json("result.json");
    json << response;
    json.close();

  }

}
