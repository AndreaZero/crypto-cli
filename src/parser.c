#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>  // For strcasecmp (POSIX)
#include <ctype.h>
#include <time.h>
#include <cjson/cJSON.h>
#include "../include/parser.h"

// Mapping of common symbols to CoinGecko IDs
static const struct {
    const char *symbol;
    const char *coingecko_id;
} symbol_map[] = {
    {"BTC", "bitcoin"},
    {"ETH", "ethereum"},
    {"BNB", "binancecoin"},
    {"SOL", "solana"},
    {"ADA", "cardano"},
    {"XRP", "ripple"},
    {"DOT", "polkadot"},
    {"DOGE", "dogecoin"},
    {"AVAX", "avalanche-2"},
    {"MATIC", "matic-network"},
    {"LINK", "chainlink"},
    {"UNI", "uniswap"},
    {"LTC", "litecoin"},
    {"ATOM", "cosmos"},
    {"ETC", "ethereum-classic"},
    {"XLM", "stellar"},
    {"ALGO", "algorand"},
    {"FIL", "filecoin"},
    {"TRX", "tron"},
    {"VET", "vechain"},
    {"ICP", "internet-computer"},
    {"THETA", "theta-token"},
    {"EOS", "eos"},
    {"AAVE", "aave"},
    {"MKR", "maker"},
    {"SUSHI", "sushi"},
    {NULL, NULL}
};

char *symbol_to_id(const char *symbol) {
    if (!symbol) {
        return NULL;
    }
    
    // Check symbol map first
    for (int i = 0; symbol_map[i].symbol != NULL; i++) {
        if (strcasecmp(symbol, symbol_map[i].symbol) == 0) {
            size_t len = strlen(symbol_map[i].coingecko_id);
            char *id = malloc(len + 1);
            if (id) {
                strncpy(id, symbol_map[i].coingecko_id, len);
                id[len] = '\0';
            }
            return id;
        }
    }
    
    // Convert to lowercase for direct CoinGecko ID match
    size_t len = strlen(symbol);
    char *id = malloc(len + 1);
    if (!id) {
        return NULL;
    }
    
    for (size_t i = 0; i < len; i++) {
        id[i] = tolower((unsigned char)symbol[i]);
    }
    id[len] = '\0';
    
    return id;
}

crypto_data_t parse_crypto_json(const char *json_string) {
    return parse_crypto_json_with_currency(json_string, "usd");
}

crypto_data_t parse_crypto_json_with_currency(const char *json_string, const char *currency) {
    crypto_data_t data = {0};
    data.success = 0;
    
    if (!json_string) {
        return data;
    }
    
    const char *curr = currency ? currency : "usd";
    
    // Store currency code
    size_t curr_len = strlen(curr);
    data.currency = malloc(curr_len + 1);
    if (data.currency) {
        strncpy(data.currency, curr, curr_len);
        data.currency[curr_len] = '\0';
    }
    
    cJSON *json = cJSON_Parse(json_string);
    if (!json) {
        return data;
    }
    
    // Get first (and typically only) key in the response
    cJSON *item = json->child;
    if (!item) {
        cJSON_Delete(json);
        return data;
    }
    
    // Extract coin ID
    if (item->string) {
        size_t id_len = strlen(item->string);
        data.id = malloc(id_len + 1);
        if (data.id) {
            strncpy(data.id, item->string, id_len);
            data.id[id_len] = '\0';
        }
    }
    
    // Build currency field names dynamically
    char price_field[32];
    char change_field[32];
    char mcap_field[32];
    char volume_field[32];
    
    snprintf(price_field, sizeof(price_field), "%s", curr);
    snprintf(change_field, sizeof(change_field), "%s_24h_change", curr);
    snprintf(mcap_field, sizeof(mcap_field), "%s_market_cap", curr);
    snprintf(volume_field, sizeof(volume_field), "%s_24h_vol", curr);
    
    // Parse price data
    cJSON *price = cJSON_GetObjectItem(item, price_field);
    if (cJSON_IsNumber(price)) {
        data.current_price = price->valuedouble;
    }
    
    // Parse price change 24h
    cJSON *change_24h = cJSON_GetObjectItem(item, change_field);
    if (cJSON_IsNumber(change_24h)) {
        data.price_change_24h = change_24h->valuedouble;
        data.price_change_percentage_24h = change_24h->valuedouble;
    }
    
    // Parse market cap
    cJSON *market_cap = cJSON_GetObjectItem(item, mcap_field);
    if (cJSON_IsNumber(market_cap)) {
        data.market_cap = market_cap->valuedouble;
    }
    
    // Parse 24h volume
    cJSON *volume_24h = cJSON_GetObjectItem(item, volume_field);
    if (cJSON_IsNumber(volume_24h)) {
        data.volume_24h = volume_24h->valuedouble;
    }
    
    // Parse last updated timestamp
    cJSON *last_updated = cJSON_GetObjectItem(item, "last_updated_at");
    if (cJSON_IsNumber(last_updated)) {
        data.last_updated_at = (long)last_updated->valuedouble;
    }
    
    // Extract symbol and name from ID
    if (data.id && strlen(data.id) > 0) {
        size_t id_len = strlen(data.id);
        
        // Create symbol (uppercase version, but handle special cases)
        // Check if we have a mapping for this ID
        const char *mapped_symbol = NULL;
        for (int i = 0; symbol_map[i].symbol != NULL; i++) {
            if (strcmp(data.id, symbol_map[i].coingecko_id) == 0) {
                mapped_symbol = symbol_map[i].symbol;
                break;
            }
        }
        
        if (mapped_symbol) {
            size_t sym_len = strlen(mapped_symbol);
            data.symbol = malloc(sym_len + 1);
            if (data.symbol) {
                strncpy(data.symbol, mapped_symbol, sym_len);
                data.symbol[sym_len] = '\0';
            }
        } else {
            // Convert to uppercase
            data.symbol = malloc(id_len + 1);
            if (data.symbol) {
                for (size_t i = 0; i < id_len; i++) {
                    data.symbol[i] = toupper((unsigned char)data.id[i]);
                }
                data.symbol[id_len] = '\0';
            }
        }
        
        // Create name from ID (capitalize first letter and replace hyphens with spaces)
        data.name = malloc(id_len * 2 + 1); // Extra space for potential replacements
        if (data.name) {
            size_t j = 0;
            int capitalize_next = 1;
            for (size_t i = 0; i < id_len; i++) {
                if (data.id[i] == '-' || data.id[i] == '_') {
                    data.name[j++] = ' ';
                    capitalize_next = 1;
                } else if (capitalize_next) {
                    data.name[j++] = toupper((unsigned char)data.id[i]);
                    capitalize_next = 0;
                } else {
                    data.name[j++] = data.id[i];
                }
            }
            data.name[j] = '\0';
        }
    }
    
    // High/Low not available in simple/price endpoint
    data.high_24h = 0.0;
    data.low_24h = 0.0;
    
    data.success = 1;
    cJSON_Delete(json);
    
    return data;
}

void free_crypto_data(crypto_data_t *data) {
    if (!data) {
        return;
    }
    
    if (data->id) {
        free(data->id);
        data->id = NULL;
    }
    
    if (data->symbol) {
        free(data->symbol);
        data->symbol = NULL;
    }
    
    if (data->name) {
        free(data->name);
        data->name = NULL;
    }
    
    if (data->currency) {
        free(data->currency);
        data->currency = NULL;
    }
}

int parse_ohlc_json(const char *json_string, crypto_data_t *data) {
    if (!json_string || !data) {
        return -1;
    }
    
    cJSON *json = cJSON_Parse(json_string);
    if (!json) {
        return -1;
    }
    
    // OHLC response is an array of arrays: [[timestamp, open, high, low, close], ...]
    if (!cJSON_IsArray(json)) {
        cJSON_Delete(json);
        return -1;
    }
    
    double max_high = 0.0;
    double min_low = 0.0;
    int first_item = 1;
    
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, json) {
        if (!cJSON_IsArray(item)) {
            continue;
        }
        
        // Each item is [timestamp, open, high, low, close]
        // Need at least 5 elements
        if (cJSON_GetArraySize(item) < 5) {
            continue;
        }
        
        cJSON *high_item = cJSON_GetArrayItem(item, 2); // high (index 2)
        cJSON *low_item = cJSON_GetArrayItem(item, 3);  // low (index 3)
        
        if (!cJSON_IsNumber(high_item) || !cJSON_IsNumber(low_item)) {
            continue;
        }
        
        double high = high_item->valuedouble;
        double low = low_item->valuedouble;
        
        if (first_item) {
            max_high = high;
            min_low = low;
            first_item = 0;
        } else {
            if (high > max_high) {
                max_high = high;
            }
            if (low < min_low) {
                min_low = low;
            }
        }
    }
    
    if (!first_item) {
        data->high_24h = max_high;
        data->low_24h = min_low;
    }
    
    cJSON_Delete(json);
    return 0;
}

markets_data_t parse_markets_json(const char *json_string, int limit) {
    markets_data_t markets = {0};
    markets.success = 0;
    markets.count = 0;
    markets.coins = NULL;
    
    if (!json_string || limit <= 0) {
        return markets;
    }
    
    cJSON *json = cJSON_Parse(json_string);
    if (!json) {
        return markets;
    }
    
    // Markets response is an array of coin objects
    if (!cJSON_IsArray(json)) {
        cJSON_Delete(json);
        return markets;
    }
    
    int array_size = cJSON_GetArraySize(json);
    int actual_limit = (array_size < limit) ? array_size : limit;
    
    markets.coins = malloc(sizeof(crypto_data_t) * actual_limit);
    if (!markets.coins) {
        cJSON_Delete(json);
        return markets;
    }
    
    // Initialize all coins
    for (int i = 0; i < actual_limit; i++) {
        markets.coins[i] = (crypto_data_t){0};
        markets.coins[i].success = 0;
    }
    
    cJSON *item = NULL;
    int index = 0;
    cJSON_ArrayForEach(item, json) {
        if (index >= actual_limit) {
            break;
        }
        
        if (!cJSON_IsObject(item)) {
            continue;
        }
        
        crypto_data_t *coin = &markets.coins[index];
        
        // Parse id
        cJSON *id_item = cJSON_GetObjectItem(item, "id");
        if (cJSON_IsString(id_item)) {
            size_t id_len = strlen(id_item->valuestring);
            coin->id = malloc(id_len + 1);
            if (coin->id) {
                strncpy(coin->id, id_item->valuestring, id_len);
                coin->id[id_len] = '\0';
            }
        }
        
        // Parse symbol
        cJSON *symbol_item = cJSON_GetObjectItem(item, "symbol");
        if (cJSON_IsString(symbol_item)) {
            size_t sym_len = strlen(symbol_item->valuestring);
            coin->symbol = malloc(sym_len + 1);
            if (coin->symbol) {
                strncpy(coin->symbol, symbol_item->valuestring, sym_len);
                coin->symbol[sym_len] = '\0';
            }
        }
        
        // Parse name
        cJSON *name_item = cJSON_GetObjectItem(item, "name");
        if (cJSON_IsString(name_item)) {
            size_t name_len = strlen(name_item->valuestring);
            coin->name = malloc(name_len + 1);
            if (coin->name) {
                strncpy(coin->name, name_item->valuestring, name_len);
                coin->name[name_len] = '\0';
            }
        }
        
        // Parse current_price
        cJSON *price_item = cJSON_GetObjectItem(item, "current_price");
        if (cJSON_IsNumber(price_item)) {
            coin->current_price = price_item->valuedouble;
        }
        
        // Parse market_cap
        cJSON *market_cap_item = cJSON_GetObjectItem(item, "market_cap");
        if (cJSON_IsNumber(market_cap_item)) {
            coin->market_cap = market_cap_item->valuedouble;
        }
        
        // Parse total_volume (24h volume)
        cJSON *volume_item = cJSON_GetObjectItem(item, "total_volume");
        if (cJSON_IsNumber(volume_item)) {
            coin->volume_24h = volume_item->valuedouble;
        }
        
        // Parse price_change_percentage_24h
        cJSON *change_pct_item = cJSON_GetObjectItem(item, "price_change_percentage_24h");
        if (cJSON_IsNumber(change_pct_item)) {
            coin->price_change_percentage_24h = change_pct_item->valuedouble;
            // Calculate absolute change from percentage
            coin->price_change_24h = coin->current_price * (coin->price_change_percentage_24h / 100.0);
        }
        
        // Parse high_24h
        cJSON *high_item = cJSON_GetObjectItem(item, "high_24h");
        if (cJSON_IsNumber(high_item)) {
            coin->high_24h = high_item->valuedouble;
        }
        
        // Parse low_24h
        cJSON *low_item = cJSON_GetObjectItem(item, "low_24h");
        if (cJSON_IsNumber(low_item)) {
            coin->low_24h = low_item->valuedouble;
        }
        
        // Parse last_updated
        cJSON *updated_item = cJSON_GetObjectItem(item, "last_updated");
        if (cJSON_IsString(updated_item)) {
            // Parse ISO 8601 timestamp (simplified - just mark as updated)
            coin->last_updated_at = time(NULL);
        }
        
        coin->success = 1;
        index++;
    }
    
    markets.count = index;
    markets.success = 1;
    
    cJSON_Delete(json);
    return markets;
}

void free_markets_data(markets_data_t *data) {
    if (!data) {
        return;
    }
    
    if (data->coins) {
        for (int i = 0; i < data->count; i++) {
            free_crypto_data(&data->coins[i]);
        }
        free(data->coins);
        data->coins = NULL;
    }
    
    data->count = 0;
    data->success = 0;
}