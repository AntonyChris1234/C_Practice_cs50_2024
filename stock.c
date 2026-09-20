#define _POSIX_C_SOURCE 200809L // for popen/pclose under -std=c11

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs50.h"

#define MAX_TICKER 16

static int ticker_is_valid(const char *s);
static char *run_curl(const char *url);
static int extract_number(const char *json, const char *key, double *out);
static int extract_string(const char *json, const char *key, char *out, size_t out_size);

int main(void)
{
    string ticker = get_string("Ticker symbol (e.g. AAPL): ");
    size_t len = ticker == NULL ? 0 : strlen(ticker);
    if (len == 0 || len > MAX_TICKER || !ticker_is_valid(ticker))
    {
        printf("Invalid ticker symbol.\n");
        return 1;
    }

    char symbol[MAX_TICKER + 1];
    for (size_t i = 0; i < len; i++)
    {
        symbol[i] = (char) toupper((unsigned char) ticker[i]);
    }
    symbol[len] = '\0';

    char url[256];
    snprintf(url, sizeof(url),
             "https://query1.finance.yahoo.com/v8/finance/chart/%s?interval=1d&range=1d",
             symbol);

    char *json = run_curl(url);
    if (json == NULL)
    {
        printf("Could not reach the network (is curl installed, and are we online?).\n");
        return 1;
    }

    if (strstr(json, "\"regularMarketPrice\"") == NULL)
    {
        printf("No data found for \"%s\". Check the symbol and try again.\n", symbol);
        free(json);
        return 1;
    }

    char name[128] = "";
    char currency[8] = "";
    double price = 0, high = 0, low = 0, prev_close = 0, volume = 0;

    extract_string(json, "\"longName\"", name, sizeof(name));
    extract_string(json, "\"currency\"", currency, sizeof(currency));
    extract_number(json, "\"regularMarketPrice\"", &price);
    extract_number(json, "\"regularMarketDayHigh\"", &high);
    extract_number(json, "\"regularMarketDayLow\"", &low);
    extract_number(json, "\"chartPreviousClose\"", &prev_close);
    extract_number(json, "\"regularMarketVolume\"", &volume);

    double change = price - prev_close;
    double percent = prev_close != 0 ? (change / prev_close) * 100 : 0;

    printf("\n%s (%s)\n", symbol, name[0] ? name : "unknown company");
    printf("  Price:          %.2f %s\n", price, currency);
    printf("  Change:         %+.2f (%+.2f%%)\n", change, percent);
    printf("  Day range:      %.2f - %.2f\n", low, high);
    printf("  Previous close: %.2f\n", prev_close);
    printf("  Volume:         %.0f\n", volume);

    free(json);
    return 0;
}

// Only letters/digits/./- are allowed, so the symbol can be safely embedded
// in a single-quoted shell string passed to curl.
static int ticker_is_valid(const char *s)
{
    for (const char *p = s; *p; p++)
    {
        if (!isalnum((unsigned char) *p) && *p != '.' && *p != '-')
        {
            return 0;
        }
    }
    return 1;
}

// Shells out to the system's curl binary and returns its stdout as a
// heap-allocated, NUL-terminated string (caller must free), or NULL on
// failure. Avoids depending on libcurl's dev headers, which aren't
// installed in this environment.
static char *run_curl(const char *url)
{
    char command[512];
    int n = snprintf(command, sizeof(command),
                      "curl -s --max-time 10 -A 'Mozilla/5.0' '%s'", url);
    if (n < 0 || (size_t) n >= sizeof(command))
    {
        return NULL;
    }

    FILE *pipe = popen(command, "r");
    if (pipe == NULL)
    {
        return NULL;
    }

    size_t capacity = 4096, length = 0;
    char *buffer = malloc(capacity);
    if (buffer == NULL)
    {
        pclose(pipe);
        return NULL;
    }

    size_t bytes_read;
    while ((bytes_read = fread(buffer + length, 1, capacity - length, pipe)) > 0)
    {
        length += bytes_read;
        if (length == capacity)
        {
            capacity *= 2;
            char *bigger = realloc(buffer, capacity);
            if (bigger == NULL)
            {
                free(buffer);
                pclose(pipe);
                return NULL;
            }
            buffer = bigger;
        }
    }
    buffer[length] = '\0';
    pclose(pipe);

    if (length == 0)
    {
        free(buffer);
        return NULL;
    }
    return buffer;
}

// Minimal, purpose-built JSON scraping: finds "key": and parses whatever
// numeric value follows. Good enough for Yahoo's flat chart-meta object,
// not a general JSON parser.
static int extract_number(const char *json, const char *key, double *out)
{
    const char *p = strstr(json, key);
    if (p == NULL)
    {
        return 0;
    }
    p = strchr(p + strlen(key), ':');
    if (p == NULL)
    {
        return 0;
    }
    *out = strtod(p + 1, NULL);
    return 1;
}

static int extract_string(const char *json, const char *key, char *out, size_t out_size)
{
    const char *p = strstr(json, key);
    if (p == NULL)
    {
        return 0;
    }
    p = strchr(p + strlen(key), ':');
    if (p == NULL)
    {
        return 0;
    }
    p = strchr(p, '"');
    if (p == NULL)
    {
        return 0;
    }
    p++;
    size_t i = 0;
    while (*p && *p != '"' && i + 1 < out_size)
    {
        if (*p == '\\' && *(p + 1))
        {
            p++;
        }
        out[i++] = *p++;
    }
    out[i] = '\0';
    return 1;
}
