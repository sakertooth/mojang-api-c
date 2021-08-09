#pragma once
#define RUN_TEST(fn, args...)                                                                       \
    curl_global_init(CURL_GLOBAL_ALL);                                                              \
                                                                                                    \
    int result = fn(args);                                                                          \
                                                                                                    \
    printf("%s\n", out);                                                                            \
    printf("%d\n", result);                                                                         \
                                                                                                    \
    free(out);                                                                                      \
    curl_global_cleanup();                                                                          \
    return result;                                                                                  