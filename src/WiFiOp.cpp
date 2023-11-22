#include "WiFIOp.h"


WiFiServer server(80);

const char* ssid = "reader_esp32";
const char* pwd = "123456";

char dest_ssid[20] = "";
char dest_pwd[20] = "";

String down_dir = "/localdown/";
String remote_ip = "192.168.137.1";


/**
 * 连接到目标wifi
*/
void wifi_connect() {

    Serial.print("Connecting to");
    Serial.println(ssid);
    WiFi.begin(ssid, pwd);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
    Serial.println("wifi start");
}

/**
 * 打印远程目录下的所有文件
*/
void http_remote_dir_show() {
    HTTPClient http;
    http.begin("http://" + remote_ip + down_dir);

    int http_code = http.GET();
    Serial.printf("访问远程目录，http请求状态: %d \n", http_code);
    String response = http.getString();

    Serial.println("响应数据：");
    Serial.println(response);
    http.end();
}

/**
 * http流写入sd中
*/
void http_file_to_sd(HTTPClient &http, String file_name) {
    // HTTPClient client = HTTPClient(http);
    int len = http.getSize();
        
    Serial.printf("file size: %d \n", len);
    Serial.printf("Byte( %d kB) \n", len/1024);
    // return http.getStreamPtr();
    WiFiClient *stream = http.getStreamPtr();

    uint8_t * buf = (uint8_t *) malloc(sizeof(uint8_t) * 256);
    String file_path = "/" + file_name;
    if (sd_file_exists(file_path)) {
        Serial.printf("文件：%s 已存在", file_path);
        return;
    }
    File wr_file = sd_file_write_ready(file_path);
    Serial.printf("buf size: %d \n", sizeof(buf));
    // String show;
    while (http.connected() && (len > 0 || len == -1)) {
        size_t size = stream -> available();
        if (size) {
            size_t read_size = (size > sizeof(buf)) ? sizeof(buf) : size;
            int c = stream -> readBytes(buf, read_size);
            // 结束标记
            buf[read_size] = 0;
            // 打印写入内容
            String buf_content = String(buf, c);
            Serial.print(buf_content);
            wr_file.println(buf_content);
            if (len > 0) {
                len -= c;
            }
        }
    }
    // 释放缓冲区
    free(buf);
    // 关闭sd文件
    wr_file.close();
}


/**
 * 通过http获取文件
*/
void get_file_and_call(String file_name, void (*file_call) (HTTPClient&, String)) {
    HTTPClient http;
    String storage_path = "http://" + remote_ip + down_dir + file_name;
    http.begin(storage_path);

    int http_code = http.GET();

    if (http_code == HTTP_CODE_OK) {
        Serial.println("http请求成功");
        file_call(http, file_name);
    } else {
        Serial.println("http请求失败");
    }

}

/**
 * 启动ap server
*/
void wifi_ap_server() {
    // ap 模式作为wifi接入点，其他设备可以连接到该节点上
    if (!WiFi.softAP(ssid, NULL)) {
        Serial.println("wifi ap服务器启动失败");
        return;
    }
    IPAddress ip = WiFi.softAPIP();
    Serial.printf("AP IP address: %s \n", ip.toString());
    server.begin();
    Serial.println("ap server started");
}

void wifi_server_end() {
    server.end();
}

void set_wifi_conn_info() {

}


/**
 * wifi客户端监听端口示例
 * @return true:接收成功，false:程序中断
*/
bool wifi_file_recv() {
    // WiFiServer server(80);
    // wifi_connect();
    // server.begin();
    // 监听客户端
    // 初始保证进入循环，get_key()的值会冗余，不一定是当前最新按钮状态
    bool init = true;
    
    // 有可能中断，退出程序
    while (init || get_key() != LEFT_ACTION) {
        init = false;
        WiFiClient client = server.available();
        if (client) {
            Serial.println("New Client");
            IPAddress client_ip = client.remoteIP();
            Serial.printf("client ip: %s \n", client_ip.toString());
            String file_name = "";
            int file_size = 0;
            String file_info = "";

            // 先读取文件信息
            while (client.connected()) {
                char c = client.read();
                if (c == ';') {
                    break;
                }
                file_info += c;
            }
            int i = file_info.indexOf(":");
                
            if (i > -1) {
                file_name = file_info.substring(0, i);
                Serial.println(file_name);
                // TODO数字格式校验
                file_size = file_info.substring(i + 1).toInt();
                Serial.println(file_size);
            } else {
                Serial.println("文件信息传输格式错误");
                return false;
            }

            // 准备读取文件内容
            String file_path = "/" + file_name;
            if (sd_file_exists(file_path)) {
                Serial.printf("文件：%s 已存在,不会覆写", file_path);
                return false;
            }

            client.println("File received ready");
            
            File wr_file = sd_file_write_ready(file_path);
            int already_read_size = 0;
            int buf_len = 256;
            char buf[buf_len] = {};
            int counter = 0;
            while (client.connected()) {
                if (client.available()) {
                    
                    char c = client.read();
                    // Serial.write(c);
                    buf[counter ++] = c;
                    already_read_size ++;
                    if (counter == buf_len - 2) {
                        buf[counter] = '\0';
                        wr_file.print(buf);
                        counter = 0;
                    }

                    if (already_read_size == file_size) {
                        if (counter != 0) {
                            buf[counter] = '\0';
                            wr_file.print(buf);
                        }
                        break;
                    }
                    
                }
            }
            wr_file.close();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.stop();
            Serial.println("Client Disconnected.");
            break;
        }
        delay(2000);
        Serial.println(".");
    }

    // 程序中断
    if (get_key() == LEFT_ACTION) {
        return false;
    }
    return true;
}

