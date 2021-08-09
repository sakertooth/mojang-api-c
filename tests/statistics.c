#include <mojang-api/mojang-api.h>
#include <apitest.h>

int main() {
    char *out = NULL;
    const char *stats[] = {
        MOJANG_API_STAT_ITEM_SOLD_MINECRAFT,
        MOJANG_API_STAT_PREPAID_CARD_REDEEMED_MINECRAFT
    };

    RUN_TEST(mojang_api_statistics, stats, sizeof(stats) / sizeof(char *), &out);
}