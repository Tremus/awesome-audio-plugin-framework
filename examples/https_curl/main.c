#include "libcurl_scu.c"

#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct curl_slist POST_HEADERS[] = {
    {.data = "Content-Type: application/json", .next = NULL}};

struct https_response
{
    CURL* session;
    char* body;
    size_t size;
    long status;
    atomic_bool cancel_request;
};

int https_progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                            curl_off_t ultotal, curl_off_t ulnow)
{
    struct https_response* res = (struct https_response*)clientp;
    if (atomic_load(&res->cancel_request))
    {
        printf("Cancelling request at size: %zu\n", res->size);
        return 1;
    }
    else
        return CURL_PROGRESSFUNC_CONTINUE;
}

static size_t https_request_cb(void* data, size_t size, size_t nmemb,
                               void* clientp)
{
    size_t realsize            = size * nmemb;
    struct https_response* res = (struct https_response*)clientp;

    char* ptr = realloc(res->body, res->size + realsize + 1);
    if (ptr == NULL)
        return 0; /* out of memory! */

    res->body = ptr;
    memcpy(&(res->body[res->size]), data, realsize);
    res->size += realsize;
    res->body[res->size] = 0;

    return realsize;
};

// Use this for simple GET requests
static CURLcode https_get(struct https_response* res, const char* url)
{
    CURL* session = res->session;
    /* send all data to this function  */
    curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, https_request_cb);
    curl_easy_setopt(session, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(session, CURLOPT_XFERINFODATA, (void*)res);
    curl_easy_setopt(session, CURLOPT_XFERINFOFUNCTION,
                     https_progress_callback);

    /* we pass our 'https_response' struct to the callback function */
    curl_easy_setopt(session, CURLOPT_WRITEDATA, (void*)res);

    curl_easy_setopt(session, CURLOPT_URL, url);

    /* send a request */
    CURLcode code = curl_easy_perform(session);

    curl_easy_getinfo(session, CURLINFO_RESPONSE_CODE, &res->status);
    return code;
}

// Use this for simple JSON POST requests
static CURLcode https_post_json(struct https_response* res, const char* url,
                                const char* postdata)
{
    CURL* session = res->session;
    /* send all data to this function  */
    curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, https_request_cb);
    curl_easy_setopt(session, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(session, CURLOPT_XFERINFODATA, (void*)res);
    curl_easy_setopt(session, CURLOPT_XFERINFOFUNCTION,
                     https_progress_callback);

    /* we pass our 'https_response' struct to the callback function */
    curl_easy_setopt(session, CURLOPT_WRITEDATA, (void*)res);

    curl_easy_setopt(session, CURLOPT_URL, url);
    curl_easy_setopt(session, CURLOPT_COPYPOSTFIELDS, postdata);
    curl_easy_setopt(session, CURLOPT_HTTPHEADER, POST_HEADERS);

    /* send a request */
    CURLcode code = curl_easy_perform(session);

    curl_easy_getinfo(session, CURLINFO_RESPONSE_CODE, &res->status);
    return code;
}

// Useful examples:
// https://curl.se/libcurl/c/https.html
// https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
int main()
{
    CURLcode code;
    struct https_response* res = malloc(sizeof(struct https_response));

    res->session = curl_easy_init();
    res->body    = NULL;
    res->size    = 0;
    res->status  = 0;
    atomic_init(&res->cancel_request, false);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Test GET
    if (res->session)
    {
        code = https_get(res, "https://example.com/");

        if (code == CURLE_OK)
        {
            printf("HTTP Status: %ld\n", res->status);
            printf("Response length: %zu\n", res->size);
            printf("Response: %s\n", res->body);
        }
        else
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(code));
        }

        /* remember to free the buffer */
        free(res->body);
        res->body = NULL;
        curl_easy_cleanup(res->session);
        res->session = 0;
        res->size    = 0;
        res->status  = 0;
        atomic_store(&res->cancel_request, false);
    }

    // Test POST
    res->session = curl_easy_init();
    if (res->session)
    {

        code = https_post_json(res, "https://httpbin.org/post",
                               "{\"greeting\": \"Bonjour\"}");

        if (code == CURLE_OK)
        {
            printf("HTTP Status: %ld\n", res->status);
            printf("Response length: %zu\n", res->size);
            printf("Response: %s\n", res->body);
        }
        else
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(code));
        }

        /* remember to free the buffer */
        free(res->body);
        res->body = NULL;
        curl_easy_cleanup(res->session);
        res->session = 0;
        res->size    = 0;
        res->status  = 0;
        atomic_store(&res->cancel_request, false);
    }
    free(res);
    res = NULL;

    curl_global_cleanup();

    return 0;
}