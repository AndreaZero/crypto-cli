# crypto-cli

A simple and fast command-line interface (CLI) tool written in C for fetching cryptocurrency prices and information from the CoinGecko API.

## Features

- 🚀 Fast and lightweight C implementation
- 💰 Real-time cryptocurrency prices
- 📊 Detailed market information (market cap, volume, 24h change)
- 🎯 Simple and intuitive CLI interface
- 🔍 Support for common cryptocurrency symbols (BTC, ETH, etc.)

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

# Example

![alt text](https://i.postimg.cc/rwKH19hf/example.gif)

### Basic Usage

Display full information for a cryptocurrency:
```bash
crypto bitcoin
crypto btc
```

Display only the price:
```bash
crypto bitcoin price
crypto btc price
```

### Examples

```bash
# Full information
./bin/crypto bitcoin
./bin/crypto eth
./bin/crypto solana

# Price only
./bin/crypto btc price
./bin/crypto ethereum price
```

### Supported Symbols

The tool supports both CoinGecko IDs (e.g., `bitcoin`, `ethereum`) and common symbols (e.g., `BTC`, `ETH`). Supported symbols include:

- BTC, ETH, BNB, SOL, ADA, XRP, DOT, DOGE, AVAX, MATIC
- LINK, UNI, LTC, ATOM, ETC, XLM, ALGO, FIL, TRX, VET
- ICP, THETA, EOS, AAVE, MKR, SUSHI

And many more via CoinGecko ID (lowercase name like `bitcoin`, `ethereum`, etc.).

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

## License

This project is open source. Choose an appropriate license (MIT, GPL, etc.) based on your preferences.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Troubleshooting

### "Failed to fetch data from API"
- Check your internet connection
- Verify that the CoinGecko API is accessible
- Ensure libcurl is properly installed

### "Cryptocurrency not found"
- Try using the CoinGecko ID (lowercase, e.g., `bitcoin`) instead of symbol
- Verify the symbol/ID is correct on CoinGecko

### Build Errors
- Ensure all dependencies are installed
- Run `make check-deps` to verify library availability
- Check that GCC and Make are installed

## Author

Vibe Coded by AndreaZero in Cursor Composer 1.
Created as an open-source project for cryptocurrency price tracking.

