### 基于esp32 开发简单墨水屏电子阅读器

#### 项目介绍

1. 使用esp32 wroom芯片，WeAct-Epaper 2.13寸墨水屏，sd模块，三个按键，实现简易的电子书阅读器，支持书籍列表，阅读（上下翻页，位置记录），删除等简单功能，并支持wifi下文件接收；菜单系统可进行简单功能扩展

2. 项目展示：

   * 整体模块：

     ![](https://github.com/liftp/reader_esp32/blob/main/image/modules.jpg)

   * 菜单列表：

     ![](https://github.com/liftp/reader_esp32/blob/main/image/menu_list.jpg)

   * 书籍列表：

     ![](https://github.com/liftp/reader_esp32/blob/main/image/book_list.jpg)

   * 阅读展示：

     ![](https://github.com/liftp/reader_esp32/blob/main/image/read.jpg)

     ![](https://github.com/liftp/reader_esp32/blob/main/image/read_show1.jpg)

     ![](https://github.com/liftp/reader_esp32/blob/main/image/read_show1.jpg)

   * 文件接收：

     ![](https://github.com/liftp/reader_esp32/blob/main/image/file_receive.jpg)

   * 图像展示：

     ![](https://github.com/liftp/reader_esp32/blob/main/image/wall_paper_menu.jpg)

     ![](https://github.com/liftp/reader_esp32/blob/main/image/wall_paper_display.jpg)

     

#### 开发框架

1. vscode+platformio+esp32，使用c语言进行实现

#### 模块介绍

1. 菜单模块
   * 一级菜单包括：书籍，文件接收，屏幕显示
   * 书籍菜单下级：展示txt书籍列表，可进行阅读、删除功能
2. sd模块
   * 连接方式：sd模块和显示模块都使用spi协议，只是对应cs引脚不同，作为一主多从片选信号使用；MOSI接23引脚，SCK接18引脚，MISO接19引脚；需要注意电源电压是否适配，这里统一采用3.3v；
   * 代码实现：主要用来处理文件内容的读写，和书籍列表等功能
3. 文件传输模块
   * 代码实现：使用socket进行文件传输，对应的远程传输功能使用java代码编写： [socket文件传输java](https://github.com/liftp/socket_file_send)
   * 需要注意的是：文件使用上，只能兼容utf-8编码的文件，否则会导致阅读乱码，虽然代码中字体使用gb2312，但是猜测问题原因是esp32系统操作编码格式只支持utf-8，所以在传输文件时可以将其他编码的文件进行格式转换为utf-8
4. wifi模块
   * 连接方式：芯片采用ESP32-WROOM，一体集成了wifi，这里不再详细说明
   * 代码实现：1.这里使用ap模式，相当于wifi服务器，用其他设备连接该wifi，缺陷就是其他设备连接使用时不能进行网络访问；2.针对前面的模式可以sta模式，去连接路由器网络，然后通网络下的设备就可以通过ip访问，这种方式的缺陷就是当前设备操作简陋，只支持三键操作，不能输入wifi的账号密码，一般只能代码写死在设备上，但是如果切换网络，又不能继续使用；3.针对前面两种情况，可以基于文件传输，实现wifi密码的配置，这样既能灵活连接wifi，也能使用网络，这里暂未实现，因为只是基于同网络下的socket文件传输，不需要网络
5. 墨水屏显示模块
   * 连接方式：SCK连接18引脚，MOSI连接23引脚，DC连接14引脚，CS片选连接4引脚（需要和sd分开），RST连接2引脚，BUSY连接15引脚，MISO对于该显示模块暂时不需要
   * 代码实现：1.需要注意使用中文显示，这里采用u8g2库，和开源[wqy字体](https://github.com/larryli/u8g2_wqy )
   * 刷新模式：这里使用的局部刷新，减少全局刷新屏幕的闪烁

6. 按键操作：
   * 简单使用三个按键，使用中断进行功能操作，左键：25引脚，中键：33引脚，右键：32引脚
   * 按键功能：左键：一般用于菜单返回，中键：用于菜单列表向下滚动选择，右键：进入选择菜单；当然对于某些功能需要特殊修改，像阅读功能，左键向上翻页，右键向下翻页，中键返回书籍列表



