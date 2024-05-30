#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "url.h"

/* Function to convert a string to lowercase
static void toLowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}*/

static bool startsWith(const char *base, const char *str) {
	return strncmp(base, str, strlen(str)) == 0;
}

char *normalizeURL(const char *base, const char *url) {
    if (url == NULL || url[0] == '\0' || base == NULL || base[0] == '\0') {
        // Empty URLs aren't valid, so return null.
        return NULL;
    }

    if (startsWith(url, "http://") || startsWith(url, "https://")) {
        // The input begins with http:// or https://, so it appears to be normalized already.
        return strdup(url);
    }

    const bool base_is_http = startsWith(base, "http://");
    const bool base_is_https = startsWith(base, "https://");

    if (!base_is_http && !base_is_https) {
        // Invalid base URL schema.
        return NULL;
    }

	// Remove trailing '#' if present
    size_t url_length = strlen(url);
    if (url_length > 0 && url[url_length - 1] == '#') {
        char *trimmed_url = malloc(url_length);  // Allocate memory without duplicating the string
        if (trimmed_url == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        strncpy(trimmed_url, url, url_length - 1);  // Copy the original string without the trailing '#'
        trimmed_url[url_length - 1] = '\0';  // Null-terminate the modified string
        url = trimmed_url;
    }

    const size_t schema_length = base_is_http ? 7 : 8;
    const char *base_without_schema = base + schema_length;
    const char *first_slash = strchr(base_without_schema, '/');

    if (first_slash == NULL) {
        // Base URL is something like "http://example.com" without a final slash.
        const bool domain_relative = url[0] == '/';
        char *out = calloc(strlen(base) + (domain_relative ? 0 : 1) + strlen(url) + 1, sizeof(char));

        if (out == NULL) {
            fprintf(stderr, "Couldn't calloc memory for normalized URL\n");
            exit(20);
        }

        strcat(out, base);
        if (!domain_relative) {
            strcat(out, "/");
        }
        strcat(out, url);

        return out;
    }

	if (url[0] == '/') { // Domain-relative.
		char *out = calloc(first_slash - base + strlen(url) + 1, sizeof(char));

		if (out == NULL) {
			fprintf(stderr, "Couldn't calloc memory for normalized URL\n");
			exit(21);
		}

		// Copy base up to and including the first slash (except for the schema).
		strncat(out, base, first_slash - base);
		strcat(out, url);
		return out;
	} else { // Path-relative.
		const char *last_slash = strrchr(base, '/');
		char *out = calloc(last_slash - base + 1 + strlen(url) + 1, sizeof(char));

		if (out == NULL) {
			fprintf(stderr, "Couldn't calloc memory for normalized URL\n");
			exit(22);
		}

		strncat(out, base, last_slash - base + 1);
		strcat(out, url);
		return out;
	}
}

bool isInternalURL(const char *base, const char *to_validate) {
	// Discard schemata.

	if (startsWith(base, "http://"))
		base += 7;
	else if (startsWith(base, "https://"))
		base += 8;
	else
		return false;

	if (startsWith(to_validate, "http://"))
		to_validate += 7;
	else if (startsWith(to_validate, "https://"))
		to_validate += 8;
	else
		return false;

	const char *base_slash = strchr(base, '/');
	const char *url_slash = strchr(to_validate, '/');

	if (base_slash == NULL) {
		if (url_slash == NULL)
			return strcmp(base, to_validate) == 0;
		return strlen(base) < strlen(to_validate) && strncmp(base, to_validate, url_slash - to_validate) == 0;
	}

	if (url_slash == NULL)
		return strlen(to_validate) < strlen(base) && strncmp(base, to_validate, base_slash - base) == 0;

	if (base_slash - base != url_slash - to_validate)
		return false;

	return strncmp(base, to_validate, base_slash - base) == 0;
}