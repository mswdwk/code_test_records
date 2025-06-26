# [SpringBoot CORS 配置详解：允许跨域请求的最佳实践](https://cloud.tencent.com/developer/article/2493415)

## 什么是跨域
跨域是指在 Web 应用中，由于浏览器的同源政策（Same-Origin Policy），不同源的网页之间进行交互时所遇到的限制。源的定义包括三个部分：协议（如 http 或 https）、域名（如 example.com）和端口（如 80 或 443）。只有当这三者都相同的时候，两个 URL 被认为是同源的。

## 为什么有同源政策？
通俗来说，浏览器厂商开发出来的浏览器都是有做安全限制的，当你打开某个网站时，浏览器就已经将请求标头中的origin属性改成了当前网站的域名。例如我访问bilibili，会是这样的一个origin，你在当前页面中做以下几种操作，均会出现跨域：

## 跨域的情形
1，http://www.bilibili.com（假设存在）

2，https://www.bilibili.com:8086(假设存在)

3，http://admin.www.bilibili.com(假设存在)

## 跨域原因解释
情况1跨域的原因是scheme（标识特定协议或资源类型的字符串）变了

情况2 跨域的原因是port（端口号）变了

情况3跨域的原因是host（域名，admin.www.bilibili.com是域名）变了

