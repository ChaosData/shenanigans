#bad idea #71382

*"don't call gradle from a rest api server, host the server from the build file"*

```bash
$ /path/to/gradle/bin/gradle build
Starting a new Gradle Daemon for this build (subsequent builds will be faster).
> Loading
> Configuring > 0/1 projects > root project
```

```bash
$ curl -v http://127.0.0.1:8081/
* Hostname was NOT found in DNS cache
*   Trying 127.0.0.1...
* Connected to 127.0.0.1 (127.0.0.1) port 8081 (#0)
> GET / HTTP/1.1
> User-Agent: curl/7.35.0
> Host: 127.0.0.1:8081
> Accept: */*
> 
< HTTP/1.1 200 OK
< Date: Tue, 15 Mar 2016 06:13:54 GMT
< Content-Type: text/html;charset=utf-8
< Content-Length: 116
* Server Jetty(9.3.8.RC0) is not blacklisted
< Server: Jetty(9.3.8.RC0)
< 
<!doctype html>
<html>
  <head>
    <meta charset="utf-8">
  </head>
  <body>
    <h1>#YOLO</h1>
  </body>
</html>

* Connection #0 to host 127.0.0.1 left intact
```
