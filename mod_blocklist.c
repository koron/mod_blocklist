#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_request.h"
#include "apr_strings.h"
#include "apr_portable.h"

#include "http_protocol.h"

#include <blocklist.h>

static int blocklist_hook(request_rec *r)
{
    // Retrieve "blocklist" from the Apache environment variables.
    const char *env = apr_table_get(r->subprocess_env, "blocklist");

    if (env != NULL) {
        apr_os_sock_t fd = -1;

        // Obtaining a client socket
        apr_socket_t *csock = ap_get_conn_socket(r->connection);

        if (csock != NULL && apr_os_sock_get(&fd, csock) == APR_SUCCESS && fd >= 0) {
            blocklist(BLOCKLIST_ABUSIVE_BEHAVIOR, (int)fd, "Apache blocklist detected via SetEnvIf");

            ap_log_rerror(APLOG_MARK, APLOG_NOTICE, 0, r,
                    "mod_blocklist: Successfully notified blocklistd for IP %s", 
                    r->useragent_ip);
        } else {
            ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
                    "mod_blocklist: Failed to get client socket descriptor");
        }
    }

    return DECLINED;
}

static void register_hooks(apr_pool_t *p)
{
    ap_hook_log_transaction(blocklist_hook, NULL, NULL, APR_HOOK_MIDDLE);
}

// Module definition structure
module AP_MODULE_DECLARE_DATA blocklist_module = {
    STANDARD20_MODULE_STUFF,
    NULL,                  /* dir config creater */
    NULL,                  /* dir merger */
    NULL,                  /* server config */
    NULL,                  /* merge server config */
    NULL,                  /* command apr_table_t */
    register_hooks         /* register hooks */
};
