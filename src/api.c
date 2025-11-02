#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../include/api.h"

#define COINGECKO_API_BASE "https://api.coingecko.com/api/v3/simple/price"
#define COINGECKO_API_OHLC_BASE "https://api.coingecko.com/api/v3/coins"
#define COINGECKO_API_MARKETS_BASE "https://api.coingecko.com/api/v3/coins/markets"

/**
 * @brief Write callback for libcurl
 */
struct write_result {
    char *data;
    size_t size;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    struct write_result *result = (struct write_result *)userp;
    
    char *ptr = realloc(result->data, result->size + total_size + 1);
    if (!ptr) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 0;
    }
    
    result->data = ptr;
    memcpy(&(result->data[result->size]), contents, total_size);
    result->size += total_size;
    result->data[result->size] = 0;
    
    return total_size;
}

char *get_api_url_with_currency(const char *symbol, const char *currency) {
    if (!symbol) {
        return NULL;
    }
    
    const char *curr = currency ? currency : "usd";
    
    size_t url_len = strlen(COINGECKO_API_BASE) + strlen(symbol) + strlen(curr) + 60;
    char *url = malloc(url_len);
    if (!url) {
        return NULL;
    }
    
    snprintf(url, url_len, "%s?ids=%s&vs_currencies=%s&include_24hr_change=true&include_market_cap=true&include_24hr_vol=true&include_last_updated_at=true", 
             COINGECKO_API_BASE, symbol, curr);
    
    return url;
}

char *get_api_url(const char *symbol) {
    return get_api_url_with_currency(symbol, "usd");
}

int fetch_crypto_data_with_currency(const char *symbol, const char *currency, char *buffer, size_t buffer_size) {
    if (!symbol || !buffer || buffer_size == 0) {
        return -1;
    }
    
    CURL *curl;
    CURLcode res;
    struct write_result result;
    
    result.data = malloc(1);
    result.size = 0;
    
    if (!result.data) {
        return -1;
    }
    
    curl = curl_easy_init();
    if (!curl) {
        free(result.data);
        return -1;
    }
    
    char *url = get_api_url_with_currency(symbol, currency);
    if (!url) {
        curl_easy_cleanup(curl);
        free(result.data);
        return -1;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&result);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "crypto-cli/1.0");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    res = curl_easy_perform(curl);
    
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    curl_easy_cleanup(curl);
    free(url);
    
    if (res != CURLE_OK) {
        free(result.data);
        return -1;
    }
    
    if (response_code != 200) {
        free(result.data);
        return -1;
    }
    
    if (result.size >= buffer_size) {
        free(result.data);
        return -1;
    }
    
    strncpy(buffer, result.data, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
    free(result.data);
    
    return 0;
}

int fetch_crypto_data(const char *symbol, char *buffer, size_t buffer_size) {
    return fetch_crypto_data_with_currency(symbol, "usd", buffer, buffer_size);
}

int fetch_ohlc_data(const char *symbol, char *buffer, size_t buffer_size) {
    if (!symbol || !buffer || buffer_size == 0) {
        return -1;
    }
    
    CURL *curl;
    CURLcode res;
    struct write_result result;
    
    result.data = malloc(1);
    result.size = 0;
    
    if (!result.data) {
        return -1;
    }
    
    curl = curl_easy_init();
    if (!curl) {
        free(result.data);
        return -1;
    }
    
    // Build OHLC URL: /coins/{id}/ohlc?vs_currency=usd&days=1
    size_t url_len = strlen(COINGECKO_API_OHLC_BASE) + strlen(symbol) + 50;
    char *url = malloc(url_len);
    if (!url) {
        curl_easy_cleanup(curl);
        free(result.data);
        return -1;
    }
    
    snprintf(url, url_len, "%s/%s/ohlc?vs_currency=usd&days=1", 
             COINGECKO_API_OHLC_BASE, symbol);
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&result);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "crypto-cli/1.0");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    res = curl_easy_perform(curl);
    
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    curl_easy_cleanup(curl);
    free(url);
    
    if (res != CURLE_OK) {
        free(result.data);
        return -1;
    }
    
    if (response_code != 200) {
        free(result.data);
        return -1;
    }
    
    if (result.size >= buffer_size) {
        free(result.data);
        return -1;
    }
    
    strncpy(buffer, result.data, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
    free(result.data);
    
    return 0;
}

int fetch_markets_data(int limit, char *buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0 || limit <= 0) {
        return -1;
    }
    
    CURL *curl;
    CURLcode res;
    struct write_result result;
    
    result.data = malloc(1);
    result.size = 0;
    
    if (!result.data) {
        return -1;
    }
    
    curl = curl_easy_init();
    if (!curl) {
        free(result.data);
        return -1;
    }
    
    // Build markets URL: /coins/markets?vs_currency=usd&order=market_cap_desc&per_page={limit}&page=1
    size_t url_len = strlen(COINGECKO_API_MARKETS_BASE) + 150; // Increased buffer size
    char *url = malloc(url_len);
    if (!url) {
        curl_easy_cleanup(curl);
        free(result.data);
        return -1;
    }
    
    snprintf(url, url_len, "%s?vs_currency=usd&order=market_cap_desc&per_page=%d&page=1&sparkline=false&price_change_percentage=24h", 
             COINGECKO_API_MARKETS_BASE, limit);
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&result);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "crypto-cli/1.0");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    res = curl_easy_perform(curl);
    
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    curl_easy_cleanup(curl);
    free(url);
    
    if (res != CURLE_OK) {
        free(result.data);
        return -1;
    }
    
    if (response_code != 200) {
        free(result.data);
        return -1;
    }
    
    if (result.size >= buffer_size) {
        free(result.data);
        return -1;
    }
    
    strncpy(buffer, result.data, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
    free(result.data);
    
    return 0;
}

