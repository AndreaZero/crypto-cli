#ifndef API_H
#define API_H

/**
 * @file api.h
 * @brief HTTP API client for CoinGecko API
 */

/**
 * @brief Fetch cryptocurrency data from CoinGecko API
 * 
 * @param symbol Cryptocurrency symbol (e.g., "bitcoin", "ethereum")
 * @param buffer Output buffer to store JSON response
 * @param buffer_size Size of the output buffer
 * @return int 0 on success, -1 on error
 */
int fetch_crypto_data(const char *symbol, char *buffer, size_t buffer_size);

/**
 * @brief Get CoinGecko API URL for a cryptocurrency symbol
 * 
 * @param symbol Cryptocurrency symbol (e.g., "bitcoin")
 * @return char* Allocated string with URL (must be freed by caller)
 */
char *get_api_url(const char *symbol);

/**
 * @brief Get CoinGecko API URL for a cryptocurrency symbol with custom currency
 * 
 * @param symbol Cryptocurrency symbol (e.g., "bitcoin")
 * @param currency Currency code (e.g., "eur", "gbp", "jpy"). If NULL, defaults to "usd"
 * @return char* Allocated string with URL (must be freed by caller)
 */
char *get_api_url_with_currency(const char *symbol, const char *currency);

/**
 * @brief Fetch cryptocurrency data from CoinGecko API with custom currency
 * 
 * @param symbol Cryptocurrency symbol (e.g., "bitcoin", "ethereum")
 * @param currency Currency code (e.g., "eur", "gbp", "jpy"). If NULL, defaults to "usd"
 * @param buffer Output buffer to store JSON response
 * @param buffer_size Size of the output buffer
 * @return int 0 on success, -1 on error
 */
int fetch_crypto_data_with_currency(const char *symbol, const char *currency, char *buffer, size_t buffer_size);

/**
 * @brief Fetch OHLC (Open, High, Low, Close) data from CoinGecko API
 * 
 * @param symbol Cryptocurrency symbol (e.g., "bitcoin", "ethereum")
 * @param buffer Output buffer to store JSON response
 * @param buffer_size Size of the output buffer
 * @return int 0 on success, -1 on error
 */
int fetch_ohlc_data(const char *symbol, char *buffer, size_t buffer_size);

/**
 * @brief Fetch markets data (top coins) from CoinGecko API
 * 
 * @param limit Number of coins to fetch (default: 10)
 * @param buffer Output buffer to store JSON response
 * @param buffer_size Size of the output buffer
 * @return int 0 on success, -1 on error
 */
int fetch_markets_data(int limit, char *buffer, size_t buffer_size);

#endif /* API_H */

