# Stage 1: Build the CGI script
FROM alpine:latest AS builder

RUN apk add --no-cache g++ libc-dev

WORKDIR /app
COPY tasks.cc .
RUN g++ -static -o tasks.cgi tasks.cc && chmod +x tasks.cgi

# Stage 2: Lighttpd runtime (no compiler)
FROM alpine:latest

RUN apk add --no-cache lighttpd

# Create CGI directory and place compiled script
RUN mkdir -p /var/www/cgi-bin
COPY --from=builder /app/tasks.cgi /var/www/cgi-bin/tasks.cgi

# Create writable tasks file
RUN touch /tmp/tasks.txt && chmod 666 /tmp/tasks.txt

# Add lighttpd config
COPY lighttpd.conf /etc/lighttpd/lighttpd.conf

# Enable CGI module (lighttpd-mod-cgi is built-in in Alpine's package)
RUN lighttpd -f /etc/lighttpd/lighttpd.conf -tt

# Expose port and start server
EXPOSE 80
CMD ["lighttpd", "-D", "-f", "/etc/lighttpd/lighttpd.conf"]
