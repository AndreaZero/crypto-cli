#include <stdio.h>
#include <string.h>
#include <strings.h>  // For strcasecmp
#include <time.h>
#include "../include/display.h"

// Get currency symbol for display
static const char *get_currency_symbol(const char *currency) {
    if (!currency) return "$";
    
    if (strcasecmp(currency, "eur") == 0) return "€";
    if (strcasecmp(currency, "gbp") == 0) return "£";
    if (strcasecmp(currency, "jpy") == 0) return "¥";
    if (strcasecmp(currency, "cny") == 0) return "¥";
    if (strcasecmp(currency, "krw") == 0) return "₩";
    if (strcasecmp(currency, "inr") == 0) return "₹";
    if (strcasecmp(currency, "btc") == 0) return "₿";
    
    // Default: return uppercase currency code or "$" for USD
    if (strcasecmp(currency, "usd") == 0) return "$";
    
    // For other currencies, return currency code uppercase
    return currency; // Will be formatted in display functions
}

void display_full_info(const crypto_data_t *data) {
    if (!data || !data->success) {
        display_error("Failed to retrieve cryptocurrency data");
        return;
    }
    
    const char *currency_symbol = get_currency_symbol(data->currency);
    
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("  %s (%s)\n", data->name ? data->name : "N/A", 
           data->symbol ? data->symbol : "N/A");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    // Format price with currency symbol
    if (strcmp(currency_symbol, "$") == 0 || strcmp(currency_symbol, "€") == 0 || 
        strcmp(currency_symbol, "£") == 0) {
        printf("  Current Price:      %s%.2f\n", currency_symbol, data->current_price);
    } else if (strcmp(currency_symbol, "¥") == 0 || strcmp(currency_symbol, "₩") == 0) {
        printf("  Current Price:      %s%.0f\n", currency_symbol, data->current_price);
    } else {
        // For other currencies, show code before price
        printf("  Current Price:      %.2f %s\n", data->current_price, 
               data->currency ? data->currency : "USD");
    }
    
    if (data->price_change_24h != 0.0) {
        const char *sign = data->price_change_24h >= 0 ? "+" : "";
        const char *color = data->price_change_24h >= 0 ? "↑" : "↓";
        
        // Format change with currency symbol
        if (strcmp(currency_symbol, "$") == 0 || strcmp(currency_symbol, "€") == 0 || 
            strcmp(currency_symbol, "£") == 0) {
            printf("  24h Change:         %s%s%.2f (%s%.2f%%)\n", 
                   sign, currency_symbol, data->price_change_24h, 
                   color, data->price_change_percentage_24h);
        } else {
            printf("  24h Change:         %s%.2f %s (%s%.2f%%)\n", 
                   sign, data->price_change_24h, 
                   data->currency ? data->currency : "USD",
                   color, data->price_change_percentage_24h);
        }
    }
    
    // Show High/Low 24h if available
    if (data->high_24h > 0.0 && data->low_24h > 0.0) {
        if (strcmp(currency_symbol, "$") == 0 || strcmp(currency_symbol, "€") == 0 || 
            strcmp(currency_symbol, "£") == 0) {
            printf("  24h High:           %s%.2f\n", currency_symbol, data->high_24h);
            printf("  24h Low:            %s%.2f\n", currency_symbol, data->low_24h);
        } else {
            printf("  24h High:           %.2f %s\n", data->high_24h, 
                   data->currency ? data->currency : "USD");
            printf("  24h Low:            %.2f %s\n", data->low_24h, 
                   data->currency ? data->currency : "USD");
        }
    }
    
    if (data->market_cap > 0) {
        const char *mcap_symbol = get_currency_symbol(data->currency);
        if (strcmp(mcap_symbol, "$") == 0 || strcmp(mcap_symbol, "€") == 0 || 
            strcmp(mcap_symbol, "£") == 0) {
            if (data->market_cap >= 1e12) {
                printf("  Market Cap:         %s%.2fT\n", mcap_symbol, data->market_cap / 1e12);
            } else if (data->market_cap >= 1e9) {
                printf("  Market Cap:         %s%.2fB\n", mcap_symbol, data->market_cap / 1e9);
            } else if (data->market_cap >= 1e6) {
                printf("  Market Cap:         %s%.2fM\n", mcap_symbol, data->market_cap / 1e6);
            } else {
                printf("  Market Cap:         %s%.2f\n", mcap_symbol, data->market_cap);
            }
        } else {
            if (data->market_cap >= 1e12) {
                printf("  Market Cap:         %.2fT %s\n", data->market_cap / 1e12, 
                       data->currency ? data->currency : "USD");
            } else if (data->market_cap >= 1e9) {
                printf("  Market Cap:         %.2fB %s\n", data->market_cap / 1e9, 
                       data->currency ? data->currency : "USD");
            } else if (data->market_cap >= 1e6) {
                printf("  Market Cap:         %.2fM %s\n", data->market_cap / 1e6, 
                       data->currency ? data->currency : "USD");
            } else {
                printf("  Market Cap:         %.2f %s\n", data->market_cap, 
                       data->currency ? data->currency : "USD");
            }
        }
    }
    
    if (data->volume_24h > 0) {
        const char *vol_symbol = get_currency_symbol(data->currency);
        if (strcmp(vol_symbol, "$") == 0 || strcmp(vol_symbol, "€") == 0 || 
            strcmp(vol_symbol, "£") == 0) {
            if (data->volume_24h >= 1e9) {
                printf("  24h Volume:         %s%.2fB\n", vol_symbol, data->volume_24h / 1e9);
            } else if (data->volume_24h >= 1e6) {
                printf("  24h Volume:         %s%.2fM\n", vol_symbol, data->volume_24h / 1e6);
            } else {
                printf("  24h Volume:         %s%.2f\n", vol_symbol, data->volume_24h);
            }
        } else {
            if (data->volume_24h >= 1e9) {
                printf("  24h Volume:         %.2fB %s\n", data->volume_24h / 1e9, 
                       data->currency ? data->currency : "USD");
            } else if (data->volume_24h >= 1e6) {
                printf("  24h Volume:         %.2fM %s\n", data->volume_24h / 1e6, 
                       data->currency ? data->currency : "USD");
            } else {
                printf("  24h Volume:         %.2f %s\n", data->volume_24h, 
                       data->currency ? data->currency : "USD");
            }
        }
    }
    
    // Market Cap to Volume ratio (indicator of activity)
    if (data->market_cap > 0 && data->volume_24h > 0) {
        double mcv_ratio = data->market_cap / data->volume_24h;
        printf("  Market Cap / Volume: %.2f\n", mcv_ratio);
    }
    
    // Last updated timestamp
    if (data->last_updated_at > 0) {
        time_t timestamp = (time_t)data->last_updated_at;
        struct tm *timeinfo = localtime(&timestamp);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
        printf("  Last Updated:        %s\n", time_str);
    }
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
}

void display_price_only(const crypto_data_t *data) {
    if (!data || !data->success) {
        display_error("Failed to retrieve cryptocurrency data");
        return;
    }
    
    const char *currency_symbol = get_currency_symbol(data->currency);
    
    if (strcmp(currency_symbol, "$") == 0 || strcmp(currency_symbol, "€") == 0 || 
        strcmp(currency_symbol, "£") == 0) {
        printf("%s%.2f\n", currency_symbol, data->current_price);
    } else if (strcmp(currency_symbol, "¥") == 0 || strcmp(currency_symbol, "₩") == 0) {
        printf("%s%.0f\n", currency_symbol, data->current_price);
    } else {
        printf("%.2f %s\n", data->current_price, data->currency ? data->currency : "USD");
    }
}

void display_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}

void display_top_coins(const markets_data_t *markets) {
    if (!markets || !markets->success || markets->count == 0) {
        display_error("Failed to retrieve top cryptocurrencies data");
        return;
    }
    
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("  Top %d Cryptocurrencies by Market Cap\n", markets->count);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("  %-4s %-8s %-20s %-12s %-15s %-15s %-10s\n", 
           "Rank", "Symbol", "Name", "Price", "Market Cap", "24h Volume", "24h Change");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    for (int i = 0; i < markets->count; i++) {
        const crypto_data_t *coin = &markets->coins[i];
        if (!coin->success) {
            continue;
        }
        
        int rank = i + 1;
        const char *symbol = coin->symbol ? coin->symbol : "N/A";
        const char *name = coin->name ? coin->name : "N/A";
        
        // Format price
        char price_str[32];
        if (coin->current_price >= 1000) {
            snprintf(price_str, sizeof(price_str), "$%.2f", coin->current_price);
        } else if (coin->current_price >= 1) {
            snprintf(price_str, sizeof(price_str), "$%.2f", coin->current_price);
        } else if (coin->current_price >= 0.01) {
            snprintf(price_str, sizeof(price_str), "$%.4f", coin->current_price);
        } else {
            snprintf(price_str, sizeof(price_str), "$%.6f", coin->current_price);
        }
        
        // Format market cap
        char mcap_str[32];
        if (coin->market_cap >= 1e12) {
            snprintf(mcap_str, sizeof(mcap_str), "$%.2fT", coin->market_cap / 1e12);
        } else if (coin->market_cap >= 1e9) {
            snprintf(mcap_str, sizeof(mcap_str), "$%.2fB", coin->market_cap / 1e9);
        } else if (coin->market_cap >= 1e6) {
            snprintf(mcap_str, sizeof(mcap_str), "$%.2fM", coin->market_cap / 1e6);
        } else {
            snprintf(mcap_str, sizeof(mcap_str), "$%.2f", coin->market_cap);
        }
        
        // Format volume
        char volume_str[32];
        if (coin->volume_24h >= 1e9) {
            snprintf(volume_str, sizeof(volume_str), "$%.2fB", coin->volume_24h / 1e9);
        } else if (coin->volume_24h >= 1e6) {
            snprintf(volume_str, sizeof(volume_str), "$%.2fM", coin->volume_24h / 1e6);
        } else {
            snprintf(volume_str, sizeof(volume_str), "$%.2f", coin->volume_24h);
        }
        
        // Format change
        char change_str[32];
        const char *change_sign = coin->price_change_percentage_24h >= 0 ? "+" : "";
        const char *change_arrow = coin->price_change_percentage_24h >= 0 ? "↑" : "↓";
        snprintf(change_str, sizeof(change_str), "%s%s%.2f%%", 
                 change_arrow, change_sign, coin->price_change_percentage_24h);
        
        // Truncate name if too long
        char name_display[21];
        size_t name_len = strlen(name);
        if (name_len > 19) {
            strncpy(name_display, name, 16);
            name_display[16] = '\0';
            strcat(name_display, "...");
        } else {
            strncpy(name_display, name, 20);
            name_display[name_len] = '\0';
        }
        
        printf("  %-4d %-8s %-20s %-12s %-15s %-15s %-10s\n", 
               rank, symbol, name_display, price_str, mcap_str, volume_str, change_str);
    }
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
}

