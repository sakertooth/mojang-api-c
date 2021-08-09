#include <mojang-api/mojang-api.h>
#include <apitest.h>

int main() {
    char *out = NULL;
    RUN_TEST(mojang_api_uuid_to_profile, "853c80ef3c3749fdaa49938b674adae6", &out);
}