#ifndef PARSER_H
#define PARSER_H

/**
 * @file parser.h
 * @brief JSON parser for cryptocurrency data
 */

/**
 * @brief Cryptocurrency data structure
 */
typedef struct {
    char *id;
    char *symbol;
    char *name;
    char *currency;  // Currency code (usd, eur, gbp, etc.)
    double current_price;
    double price_change_24h;
    double price_change_percentage_24h;
    double market_cap;
    double volume_24h;
    double high_24h;
    double low_24h;
    long last_updated_at;
    int success;
} crypto_data_t;

/**
 * @brief Parse JSON response from CoinGecko API
 * 
 * @param json_string JSON response string
 * @param currency Currency code used (e.g., "usd", "eur", "gbp"). If NULL, defaults to "usd"
 * @return crypto_data_t Parsed cryptocurrency data structure
 */
crypto_data_t parse_crypto_json_with_currency(const char *json_string, const char *currency);

/**
 * @brief Parse JSON response from CoinGecko API (backward compatibility - uses USD)
 * 
 * @param json_string JSON response string
 * @return crypto_data_t Parsed cryptocurrency data structure
 */
crypto_data_t parse_crypto_json(const char *json_string);

/**
 * @brief Free memory allocated for crypto_data_t structure
 * 
 * @param data Pointer to crypto_data_t structure to free
 */
void free_crypto_data(crypto_data_t *data);

/**
 * @brief Convert symbol to CoinGecko ID (lowercase)
 * 
 * @param symbol Cryptocurrency symbol (e.g., "BTC")
 * @return char* Allocated lowercase string (must be freed by caller)
 */
char *symbol_to_id(const char *symbol);

/**
 * @brief Parse OHLC JSON response and update high/low 24h values
 * 
 * @param json_string OHLC JSON response string (array of [timestamp, open, high, low, close])
 * @param data Pointer to crypto_data_t structure to update with high/low values
 * @return int 0 on success, -1 on error
 */
int parse_ohlc_json(const char *json_string, crypto_data_t *data);

/**
 * @brief Markets data structure (array of cryptocurrencies)
 */
typedef struct {
    crypto_data_t *coins;
    int count;
    int success;
} markets_data_t;

/**
 * @brief Parse markets JSON response (array of coin objects)
 * 
 * @param json_string Markets JSON response string
 * @param limit Maximum number of coins to parse
 * @return markets_data_t Parsed markets data structure
 */
markets_data_t parse_markets_json(const char *json_string, int limit);

/**
 * @brief Free memory allocated for markets_data_t structure
 * 
 * @param data Pointer to markets_data_t structure to free
 */
void free_markets_data(markets_data_t *data);

#endif /* PARSER_H */

