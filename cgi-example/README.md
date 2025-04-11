# CGI Example

Example of CGI script written in C++

To run this code in docker, use the following commands:

```bash
docker build -t cgi-tasks .
docker run -p 8080:80 cgi-tasks
```

and then, open [http://localhost:8080/tasks.cgi](http://localhost:8080/tasks.cgi) in your browser.
