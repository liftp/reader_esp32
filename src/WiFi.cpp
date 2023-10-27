#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SDModel.h>


WiFiServer server(80);

const char* ssid = "******";
const char* pwd = "******";

String down_dir = "/localdown/";
String remote_ip = "192.168.137.1";


/**
 * wifi连接
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
        Serial.printf("文件：%s 不存在", file_path);
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
            // 追加内容到sd
            // tf.appendBinToSd(storage_path.c_str(), buf, read_size);
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
    if (!WiFi.softAP(ssid, NULL)) {
        Serial.println("wifi ap服务器启动失败");
        return;
    }
    IPAddress ip = WiFi.softAPIP();
    Serial.printf("AP IP address: %s \n", ip.toString());
    server.begin();
    Serial.println("ap server started");

}

// void setup() {
//     Serial.begin(9600);
//     wifi_connect();

    // sd_spi_and_setup();
    // http_remote_dir_show();
    // get_file_and_call("list.txt", &http_file_to_sd);
    // wifi_ap_server();
// }



/**
 * wifi客户端监听端口示例
*/
void wifi_listen() {
    // WiFiServer server(80);
    // wifi_connect();
    // server.begin();
    // 监听客户端
    WiFiClient client = server.available();

    if (client) {
        Serial.println("New Client");
        IPAddress client_ip = client.remoteIP();
        Serial.printf("client ip: %s \n", client_ip.toString());
        String currLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                
                if (c == '\n') {
                    
                    // 结束标志
                    if (currLine.equals("###file EOF###")) {
                        Serial.println("读取结束");
                        break;
                    } else {
                        currLine += c;
                        // TODO 读整行
                        // 并清空内容
                        currLine = "";
                    }
                } else if (c != '\n') {
                    currLine += c;
                }
            }
        }
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.stop();
        Serial.println("Client Disconnected.");
    }

    delay(2000);
}

// void loop() {
//     wifi_listen();
// }