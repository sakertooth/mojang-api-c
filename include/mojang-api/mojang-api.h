#pragma once
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#define MOJANG_API_STAT_ITEM_SOLD_MINECRAFT "item_sold_minecraft"
#define MOJANG_API_STAT_PREPAID_CARD_REDEEMED_MINECRAFT "prepaid_card_redeemed_minecraft"
#define MOJANG_API_STAT_ITEM_SOLD_COBALT "item_sold_cobalt"
#define MOJANG_API_STAT_ITEM_SOLD_SCROLLS "item_sold_scrolls"
#define MOJANG_API_STAT_PREPAID_CARD_REDEEMED_COBALT "prepaid_card_redeemed_cobalt"
#define MOJANG_API_STAT_ITEM_SOLD_DUNGEONS "item_sold_dungeons"

struct __mojang_api_buffer {
    char *str;
    size_t size;
};

size_t __mojang_api_write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    
    struct __mojang_api_buffer *buf = (struct __mojang_api_buffer *)userp;

    char *newstr = realloc(buf->str, buf->size + nmemb + 1);
    if (!newstr) {
        fprintf(stderr, "out of memory (realloc returned NULL)\n");
        return 0;
    }

    buf->str = newstr;
    memcpy(&buf->str[buf->size], buffer, nmemb);
    buf->size += nmemb;
    buf->str[buf->size] = '\0';
    
    return nmemb;
}

void __mojang_api_init_buffer(struct __mojang_api_buffer *buf) {
    buf->str = malloc(1);
    buf->size = 0;
}

void __mojang_api_setup_request(CURL *handle, struct __mojang_api_buffer *buf, const char *url) {
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, __mojang_api_write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)buf);
}

int __mojang_api_send_free(CURL *handle, struct __mojang_api_buffer *buf, char **out) {
    int result = curl_easy_perform(handle);
    *out = buf->str;
    curl_easy_cleanup(handle);

    return result;
}

/**
 * Gets the status of Mojang's services.
 * Returns 0 for success, and non-zero for failure.
 * */
int mojang_api_status(char **out) {
    
    CURL *handle = curl_easy_init();
    struct __mojang_api_buffer buf;

    __mojang_api_init_buffer(&buf);
    __mojang_api_setup_request(handle, &buf, "https://status.mojang.com/check");
    return __mojang_api_send_free(handle, &buf, out);
}

/** 
 * Gets the UUID of a Minecraft username.
 * Returns 0 for success, and non-zero for failure.
*/
int mojang_api_username_to_uuid(const char *username, char **out) {
    
    CURL *handle = curl_easy_init();
    struct __mojang_api_buffer buf;

    char *endpoint = "https://api.mojang.com/users/profiles/minecraft";
    char *url = malloc(strlen(endpoint) + 1 + strlen(username) + 1);
    sprintf(url, "%s/%s", endpoint, username);

    __mojang_api_init_buffer(&buf);
    __mojang_api_setup_request(handle, &buf, url);

    free(url);
    return __mojang_api_send_free(handle, &buf, out);
}

/** Gets the UUIDs for multiple Minecraft usernames.
 * 'username_len' is the number of usernames present.
 *  Returns 0 for success, and non-zero for failure.
 * */ 
int mojang_api_usernames_to_uuids(const char **usernames, const size_t username_len, char **out) {
    
    size_t username_len_all = 0;
    for (size_t i = 0; i < username_len; ++i) {
        username_len_all += strlen(usernames[i]);
    }

    char *payload = malloc(
                            2 /* brackets */ + 
                            2 * username_len /* quotes */ + 
                            username_len - 1 /* commas */ + 
                            username_len_all /* usernames */ + 
                            1 /* terminator */);
    
    strcpy(payload, "[");
    for (size_t i = 0; i < username_len; ++i) {
        sprintf(payload, i < username_len - 1 ? "%s\"%s\"," : "%s\"%s\"]", payload, usernames[i]);    
    }

    CURL *handle = curl_easy_init();

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    struct __mojang_api_buffer buf;
    __mojang_api_init_buffer(&buf);

    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, payload);

    __mojang_api_setup_request(handle, &buf, "https://api.mojang.com/profiles/minecraft");
    int result = __mojang_api_send_free(handle, &buf, out);

    free(payload);
    curl_slist_free_all(headers);

    return result;
}

/**
 * Gets the name history from a UUID.
 * Returns 0 for success, and non-zero for failure.
 * */
int mojang_api_uuid_to_name_history(const char *uuid, char **out) {

    char *url = malloc(strlen("https://api.mojang.com/user/profiles/") + strlen(uuid) + strlen("/names") + 1);
    sprintf(url, "%s%s%s", "https://api.mojang.com/user/profiles/", uuid, "/names");

    CURL *handle = curl_easy_init();

    struct __mojang_api_buffer buf;
    __mojang_api_init_buffer(&buf);
    __mojang_api_setup_request(handle, &buf, url);
    
    free(url);
    return __mojang_api_send_free(handle, &buf, out);
}

/**
 * Gets the profile from a Minecraft UUID.
 * Returns 0 for success, and non-zero for failure.
 * */
int mojang_api_uuid_to_profile(const char *uuid, char **out) {

    const char *endpoint = "https://sessionserver.mojang.com/session/minecraft/profile";
    char *url = malloc(strlen(endpoint) + strlen(uuid) + 1);
    sprintf(url, "%s/%s", endpoint, uuid);

    CURL *handle = curl_easy_init();
 
    struct __mojang_api_buffer buf;
    __mojang_api_init_buffer(&buf);
    __mojang_api_setup_request(handle, &buf, url);
    
    free(url);
    return __mojang_api_send_free(handle, &buf, out);
}

/**
 * Gets the SHA1 hashes of Mojang's blocked servers.
 * Returns 0 for success, and non-zero for failure.
 * */
int mojang_api_blocked_servers(char **out) {
    
    CURL *handle = curl_easy_init();
    
    struct __mojang_api_buffer buf;
    __mojang_api_init_buffer(&buf);
    __mojang_api_setup_request(handle, &buf, "https://sessionserver.mojang.com/blockedservers");
    return __mojang_api_send_free(handle, &buf, out);
}

/**
 * Get statistics on the sales of Minecraft.
 * Returns 0 for success, and non-zero for failure. 
 * */
int mojang_api_statistics(const char **stats, const size_t stats_len, char **out) {
    
    size_t stats_len_all = 0;
    for (size_t i = 0; i < stats_len; ++i) {
        stats_len_all += strlen(stats[i]);
    }
    
    char *payload = malloc(
                            15 + // {"metricKeys":[
                            2 * stats_len + // double quotes
                            stats_len - 1 + // commas
                            stats_len_all + // stats
                            2 + // }]
                            1 // terminator
                        );

    strcpy(payload, "{\"metricKeys\":[");
    for (size_t i = 0; i < stats_len; ++i) {
        sprintf(payload, i < stats_len - 1 ? "%s\"%s\"," : "%s\"%s\"]}", payload, stats[i]);    
    }
    
    CURL *handle = curl_easy_init();

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    struct __mojang_api_buffer buf;
    __mojang_api_init_buffer(&buf);

    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, payload);

    __mojang_api_setup_request(handle, &buf, "https://api.mojang.com/orders/statistics");
    int result = __mojang_api_send_free(handle, &buf, out);

    free(payload);
    curl_slist_free_all(headers);

    return result;
}