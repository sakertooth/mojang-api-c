#include <mojang-api/mojang-api.h>
#include <apitest.h>

int main() {
    char *out = NULL;
    RUN_TEST(mojang_api_username_to_uuid, "jeb_", &out);
}