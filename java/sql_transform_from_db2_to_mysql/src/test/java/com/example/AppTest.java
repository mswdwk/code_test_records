package com.example;

import org.junit.Test;

import java.text.SimpleDateFormat;
import java.time.*;
import java.util.Calendar;

/**
 * Unit test for simple App.
 */
public class AppTest {
    /**
     * Rigorous Test :-)
     */
    @Test
    public void timeTest() throws Exception {
        // 获取当前时间的时间戳（毫秒）
        long currentTimeMillis = System.currentTimeMillis();

        // 计算一天的时间戳差值（毫秒）
        long oneDayMillis = 24 * 60 * 60 * 1000;

        // 从当前时间戳中减去一天的时间戳差值，得到一天前的时间戳
        long previousDayTimestamp = currentTimeMillis - oneDayMillis;

        // 输出一天前的时间戳
        System.out.println("一天前的时间戳: " + previousDayTimestamp);

        // 获取当前的UTC时间戳
        Instant now = Instant.now();
        System.out.println("Current UTC Timestamp (seconds): " + now.getEpochSecond());
        System.out.println("Current UTC Timestamp (milliseconds): " + now.toEpochMilli());

        // 转换为带有时区的日期时间表示
        ZonedDateTime zdt = now.atZone(ZoneId.of("UTC"));
        System.out.println("UTC Date and Time: " + zdt);

        LocalDateTime time1 = LocalDateTime.now(); // 获取当前时间
        Thread.sleep(1000); // 等待1秒
        LocalDateTime time2 = LocalDateTime.now(); // 获取当前时间+1秒
        Duration duration = Duration.between(time1, time2); // 计算两个时间点之间的差异
        System.out.println("Difference in seconds: " + duration.getSeconds()); // 以秒为单位输出差异
        System.out.println("Difference in milliseconds: " + duration.toMillis()); // 以毫秒为单位输出差异

        Calendar calendar = Calendar.getInstance();
        // calendar.setTimeZone();
        System.out.println("time zone " + ZoneId.systemDefault());
        calendar.setTimeInMillis(currentTimeMillis);
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String formattedDate = sdf.format(calendar.getTime()); // 格式化输出

        System.out.println(" formattedDate 1 = " + formattedDate);
        System.out.println(" formattedDate current  = " + sdf.format(currentTimeMillis));
        System.out.println(" formattedDate previous = " + sdf.format(previousDayTimestamp));
    }
}
