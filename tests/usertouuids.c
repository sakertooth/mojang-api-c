#include <mojang-api/mojang-api.h>
#include <apitest.h>

int main() {
    char *out = NULL;
    const char *usernames[] = {"foo", "bar", "jeb_"};
    RUN_TEST(mojang_api_usernames_to_uuids, usernames, sizeof(usernames) / sizeof(char *), &out);
}