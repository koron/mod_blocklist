# Blocklist module for Apache httpd

An extension module for Apache 2.4.x that notifies `blocklistd(8)` via
`blocklist(3)` of the IP address associated with a request for which the
`blocklist` environment variable has been set. It can be used in conjunction
with `pf(4)` to block IP addresses that send malicious requests.

Developed and tested on FreeBSD 15.1.

## Getting Started

Build:

```
make
```

Install:

```
sudo make install
```

In your `/usr/local/etc/apache24/httpd.conf` or a related configuration file,
set the `blocklist` environment variable for suspicious requests using
`SetEnvIf` or `RewriteCond`.

Example with `SetEnvIf`:

```apache
SetEnvIf Request_URI "MULCIOUS_URI_PATTERN" blocklist=1
```

Example with `RewriteCond`:

```apache
RewriteEngine On
RewriteCond %{QUERY_STRING} "MALCIOUS_QUERY_PATTERN" [NC]
RewriteRule ^ - [E=blocklist:1]
```

You can also reuse any directive that allows you to set environment variables.
