# crypto-cli v1.1.0 by AndreaZero

A simple and fast command-line interface (CLI) tool written in C for fetching cryptocurrency prices and information from the CoinGecko API.

## Features

- 🚀 Fast and lightweight C implementation
- 💰 Real-time cryptocurrency prices
- 📊 Detailed market information (market cap, volume, 24h change, high/low)
- 🌍 Multi-currency support (USD, EUR, GBP, JPY, and more)
- 📈 Top cryptocurrencies ranking by market cap
- 🎯 Simple and intuitive CLI interface
- 🔍 Support for common cryptocurrency symbols (BTC, ETH, etc.)
- 📉 24h High/Low price tracking

## Installation

### Prerequisites

You need to have the following libraries installed:

- **libcurl** - For HTTP requests
- **libcjson** - For JSON parsing
- **GCC** or compatible C compiler
- **Make** - For building

#### Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev libcjson-dev build-essential
```

#### Fedora/RHEL:
```bash
sudo dnf install libcurl-devel libcjson-devel gcc make
```

#### macOS (using Homebrew):
```bash
brew install curl libcjson
```

#### Windows:
You can use MinGW or MSYS2 to install the required libraries:
```bash
pacman -S mingw-w64-x86_64-curl mingw-w64-x86_64-cjson
```

### Building

1. Clone the repository:
```bash
git clone <repository-url>
cd crypto-cli
```

2. Build the project:
```bash
make
```

3. (Optional) Install globally to use `crypto` from anywhere:
```bash
sudo make install
```

This will install the binary to `/usr/local/bin/crypto`, making it available globally. To uninstall:
```bash
sudo make uninstall
```

For alternative installation methods (PATH, alias), see [INSTALL.md](INSTALL.md).

## Usage

### Example

![alt text](https://i.postimg.cc/rwKH19hf/example.gif)

### Basic Commands

**Display full information for a cryptocurrency:**
```bash
crypto bitcoin
crypto btc
```

**Display only the price:**
```bash
crypto bitcoin price
crypto btc price
```

**Display price in different currency:**
```bash
crypto bitcoin EUR
crypto btc GBP
crypto ethereum JPY
```

**Show top cryptocurrencies by market cap:**
```bash
crypto top          # Top 10 (default)
crypto top 20       # Top 20
crypto top 5        # Top 5
```

### Examples

```bash
# Full information
crypto bitcoin
crypto btc
crypto ethereum

# Price only
crypto btc price
crypto ethereum price

# Multi-currency support
crypto bitcoin EUR
crypto btc GBP
crypto eth JPY
crypto solana CAD

# Top cryptocurrencies
crypto top          # Top 10
crypto top 20       # Top 20
crypto top 50       # Top 50
```

### Supported Symbols

The tool supports both CoinGecko IDs (e.g., `bitcoin`, `ethereum`) and common symbols (e.g., `BTC`, `ETH`). Supported symbols include:

- BTC, ETH, BNB, SOL, ADA, XRP, DOT, DOGE, AVAX, MATIC
- LINK, UNI, LTC, ATOM, ETC, XLM, ALGO, FIL, TRX, VET
- ICP, THETA, EOS, AAVE, MKR, SUSHI

And many more via CoinGecko ID (lowercase name like `bitcoin`, `ethereum`, etc.).

### Supported Currencies

The tool supports all currencies available on CoinGecko. Common currencies include:

- **Fiat currencies**: USD, EUR, GBP, JPY, CNY, KRW, INR, CAD, AUD, CHF, BRL, and more
- **Cryptocurrencies**: BTC, ETH, and other crypto pairs

Currency codes are case-insensitive (e.g., `EUR`, `eur`, `Eur` all work).

### Command Options

- `--help`, `-h` - Display help message
- `--version`, `-v` - Display version information

## Output Format

### Full Information
```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Bitcoin (BTC)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Current Price:      $50000.00
  24h Change:         +$500.00 (+1.00%)
  24h High:           $51000.00
  24h Low:            $49000.00
  Market Cap:         $1.00T
  24h Volume:         $50.00B
  Market Cap / Volume: 20.00
  Last Updated:       2024-01-15 14:30:45
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

### Price Only
```
$50000.00
```

### Multi-Currency Output
```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Bitcoin (BTC)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Current Price:      €45000.00
  24h Change:         +€450.00 (+1.00%)
  Market Cap:         €900.00B
  24h Volume:         €45.00B
  ...
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

### Top Cryptocurrencies Table
```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Top 10 Cryptocurrencies by Market Cap
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Rank Symbol  Name                 Price        Market Cap      24h Volume     24h Change
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  1    BTC     Bitcoin              $50000.00    $1.00T          $50.00B        ↑+1.00%
  2    ETH     Ethereum             $3000.00     $360.00B        $20.00B        ↑+2.50%
  3    BNB     Binance Coin         $500.00      $75.00B         $5.00B         ↓-0.50%
  ...
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

## Development

### Building for Debug
```bash
make debug
```

### Checking Dependencies
```bash
make check-deps
```

### Cleaning Build Artifacts
```bash
make clean
```

## Project Structure

```
crypto-cli/
├── src/
│   ├── main.c      # Entry point and CLI argument parsing
│   ├── api.c       # HTTP API client for CoinGecko
│   ├── parser.c    # JSON parsing and data extraction
│   └── display.c   # Output formatting
├── include/
│   ├── api.h       # API client header
│   ├── parser.h    # Parser header
│   └── display.h   # Display header
├── Makefile        # Build configuration
└── README.md       # This file
```

## API

This tool uses the [CoinGecko API](https://www.coingecko.com/en/api), which is free and doesn't require an API key for basic usage. The tool respects CoinGecko's rate limits.

### Endpoints Used

- `/simple/price` - Get cryptocurrency prices and market data
- `/coins/{id}/ohlc` - Get OHLC (Open, High, Low, Close) data for 24h high/low tracking
- `/coins/markets` - Get top cryptocurrencies by market cap

All endpoints are part of CoinGecko's free tier and don't require authentication.

## License

This project is open source. Choose an appropriate license (MIT, GPL, etc.) based on your preferences.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Troubleshooting

### "Failed to fetch data from API"
- Check your internet connection
- Verify that the CoinGecko API is accessible
- Ensure libcurl is properly installed
- Check if you're hitting CoinGecko's rate limits (free tier: ~10-50 requests/minute)

### "Cryptocurrency not found"
- Try using the CoinGecko ID (lowercase, e.g., `bitcoin`) instead of symbol
- Verify the symbol/ID is correct on CoinGecko
- Check spelling and try common variations

### "Invalid currency" or Currency Not Supported
- Verify the currency code is correct (3-letter ISO code, e.g., `EUR`, `GBP`, `JPY`)
- Currency codes are case-insensitive
- Check CoinGecko's supported currencies list

### Build Errors
- Ensure all dependencies are installed
- Run `make check-deps` to verify library availability
- Check that GCC and Make are installed
- For WSL/Windows: Ensure you're using the correct package manager

## Author

Vibe Coded by AndreaZero in Cursor Composer 1.
Created as an open-source project for cryptocurrency price tracking.

