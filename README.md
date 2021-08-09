# mojang-api-c
A lightweight, header-only Mojang API wrapper in C.

## Why?
* This was mainly meant for me to get used to the curl library, but I also wanted to work with stuff like CMake as well.

## Caveats
* This wrapper doesnt parse the JSON given back from the API.
Instead, it is left as is for simplicity's sake.

## Todo
* Support for endpoints requiring authencation.

## Dependencies
* Curl: https://github.com/curl/curl
