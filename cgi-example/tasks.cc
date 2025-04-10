#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string url_decode(const std::string &str) {
  std::string result;
  char ch;
  int i, ii;
  for (i = 0; i < str.length(); i++) {
    if (str[i] == '+')
      result += ' ';
    else if (str[i] == '%' && i + 2 < str.length()) {
      sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
      ch = static_cast<char>(ii);
      result += ch;
      i = i + 2;
    } else {
      result += str[i];
    }
  }
  return result;
}

std::string get_post_data() {
  std::string data;
  char *len_str = getenv("CONTENT_LENGTH");
  if (len_str) {
    int len = atoi(len_str);
    data.resize(len);
    std::cin.read(&data[0], len);
  }
  return data;
}

void save_task(const std::string &task) {
  std::ofstream file("/tmp/tasks.txt", std::ios::app);
  if (file.is_open()) {
    file << task << "\n";
    file.close();
  }
}

std::vector<std::string> load_tasks() {
  std::vector<std::string> tasks;
  std::ifstream file("/tmp/tasks.txt");
  std::string line;
  while (std::getline(file, line)) {
    tasks.push_back(line);
  }
  return tasks;
}

int main() {
  std::string request_method =
      getenv("REQUEST_METHOD") ? getenv("REQUEST_METHOD") : "";

  if (request_method == "POST") {
    std::string post_data = get_post_data();
    size_t pos = post_data.find("task=");
    if (pos != std::string::npos) {
      std::string task = post_data.substr(pos + 5);
      save_task(url_decode(task));
    }
  }

  std::vector<std::string> tasks = load_tasks();

  std::cout << "Content-type: text/html\r\n\r\n";
  std::cout << "<!doctype html>\n<html lang=\"en\">\n"
            << "<head>\n<meta charset=\"UTF-8\" />\n"
            << "<meta name=\"viewport\" content=\"width=device-width, "
               "initial-scale=1.0\" />\n"
            << "<title>To do tasks</title>\n</head>\n<body>\n"
            << "<h1>Your tasks</h1>\n<ul>\n";

  for (const auto &task : tasks) {
    std::cout << "<li>" << task << "</li>\n";
  }

  std::cout << "</ul>\n"
            << "<form method=\"POST\">\n"
            << "<div>\n<label>New task:\n<input name=\"task\" />\n</label>\n"
            << "<button type=\"submit\">Add new task</button>\n"
            << "</div>\n</form>\n"
            << "</body>\n</html>\n";

  return 0;
}
