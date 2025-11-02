#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>
#include "../include/api.h"
#include "../include/parser.h"
#include "../include/display.h"

#define BUFFER_SIZE 4096
#define VERSION "1.0.0"

static void print_usage(const char *program_name) {
    printf("Usage: %s [SYMBOL] [COMMAND] | %s top [N]\n\n", program_name, program_name);
    printf("Commands:\n");
    printf("  [SYMBOL]              Display full cryptocurrency information\n");
    printf("  [SYMBOL] price        Display only the current price\n");
    printf("  [SYMBOL] [CURRENCY]   Display price in different currency (EUR, GBP, JPY, etc.)\n");
    printf("  top [N]               Display top N cryptocurrencies by market cap (default: 10)\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s bitcoin            Show full info for Bitcoin\n", program_name);
    printf("  %s btc price          Show only the price for Bitcoin\n", program_name);
    printf("  %s bitcoin EUR        Show Bitcoin price in EUR\n", program_name);
    printf("  %s btc GBP            Show Bitcoin price in GBP\n", program_name);
    printf("  %s top               Show top 10 cryptocurrencies\n", program_name);
    printf("  %s top 20            Show top 20 cryptocurrencies\n", program_name);
    printf("\n");
    printf("Version: %s\n", VERSION);
}

static void print_version(void) {
    printf("crypto-cli version %s\n", VERSION);
}

int main(int argc, char *argv[]) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    int show_price_only = 0;
    char *symbol = NULL;
    
    // Parse arguments
    if (argc < 2) {
        print_usage(argv[0]);
        curl_global_cleanup();
        return 1;
    }
    
    // Check for version flag
    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
        print_version();
        curl_global_cleanup();
        return 0;
    }
    
    // Check for help flag
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_usage(argv[0]);
        curl_global_cleanup();
        return 0;
    }
    
    // Check if command is "top"
    if (strcmp(argv[1], "top") == 0) {
        int limit = 10; // default
        
        // Parse optional limit parameter
        if (argc >= 3) {
            limit = atoi(argv[2]);
            if (limit <= 0 || limit > 250) {
                display_error("Limit must be between 1 and 250");
                curl_global_cleanup();
                return 1;
            }
        }
        
        if (argc > 3) {
            display_error("Too many arguments for 'top' command");
            print_usage(argv[0]);
            curl_global_cleanup();
            return 1;
        }
        
        // Fetch markets data
        char buffer[BUFFER_SIZE * 4] = {0}; // Larger buffer for markets data
        int result = fetch_markets_data(limit, buffer, sizeof(buffer));
        
        if (result != 0) {
            display_error("Failed to fetch markets data from API. Please check your internet connection and try again.");
            curl_global_cleanup();
            return 1;
        }
        
        // Parse markets JSON response
        markets_data_t markets = parse_markets_json(buffer, limit);
        
        if (!markets.success) {
            display_error("Failed to parse markets API response");
            curl_global_cleanup();
            return 1;
        }
        
        // Display top coins
        display_top_coins(&markets);
        
        // Cleanup
        free_markets_data(&markets);
        curl_global_cleanup();
        return 0;
    }
    
    symbol = argv[1];
    
    // Check if second argument is "price" or a currency code
    char *currency = NULL;
    if (argc >= 3) {
        if (strcmp(argv[2], "price") == 0) {
            show_price_only = 1;
        } else {
            // Assume it's a currency code (convert to lowercase)
            size_t curr_len = strlen(argv[2]);
            currency = malloc(curr_len + 1);
            if (currency) {
                for (size_t i = 0; i < curr_len; i++) {
                    currency[i] = tolower((unsigned char)argv[2][i]);
                }
                currency[curr_len] = '\0';
            }
        }
    }
    
    if (argc > 3 && strcmp(argv[2], "price") != 0) {
        fprintf(stderr, "Error: Too many arguments\n");
        print_usage(argv[0]);
        if (currency) free(currency);
        curl_global_cleanup();
        return 1;
    }
    
    // Convert symbol to CoinGecko ID format
    char *coin_id = symbol_to_id(symbol);
    if (!coin_id) {
        display_error("Invalid symbol");
        if (currency) free(currency);
        curl_global_cleanup();
        return 1;
    }
    
    // Fetch data from API
    char buffer[BUFFER_SIZE] = {0};
    int result = fetch_crypto_data_with_currency(coin_id, currency, buffer, BUFFER_SIZE);
    
    if (result != 0) {
        display_error("Failed to fetch data from API. Please check your internet connection and try again.");
        free(coin_id);
        if (currency) free(currency);
        curl_global_cleanup();
        return 1;
    }
    
    // Check if response is empty or error
    if (strlen(buffer) == 0 || strstr(buffer, "error") != NULL) {
        display_error("Cryptocurrency not found or invalid symbol");
        free(coin_id);
        if (currency) free(currency);
        curl_global_cleanup();
        return 1;
    }
    
    // Parse JSON response
    crypto_data_t crypto_data = parse_crypto_json_with_currency(buffer, currency);
    
    if (!crypto_data.success) {
        display_error("Failed to parse API response");
        free(coin_id);
        if (currency) free(currency);
        curl_global_cleanup();
        return 1;
    }
    
    // Fetch OHLC data to get high/low 24h (only if currency is USD or NULL)
    // Note: OHLC endpoint only supports USD, so we skip it for other currencies
    if (!currency || strcmp(currency, "usd") == 0) {
        char ohlc_buffer[BUFFER_SIZE] = {0};
        int ohlc_result = fetch_ohlc_data(coin_id, ohlc_buffer, BUFFER_SIZE);
        if (ohlc_result == 0) {
            // Parse OHLC data and update high/low values
            parse_ohlc_json(ohlc_buffer, &crypto_data);
        }
    }
    
    // Display data
    if (show_price_only) {
        display_price_only(&crypto_data);
    } else {
        display_full_info(&crypto_data);
    }
    
    // Cleanup
    free_crypto_data(&crypto_data);
    free(coin_id);
    if (currency) free(currency);
    curl_global_cleanup();
    
    return 0;
}

