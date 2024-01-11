import io.prometheus.metrics.core.datapoints.Timer;
import io.prometheus.metrics.core.metrics.Counter;
import io.prometheus.metrics.core.metrics.Gauge;
import io.prometheus.metrics.core.metrics.Histogram;
import io.prometheus.metrics.exporter.httpserver.HTTPServer;
import io.prometheus.metrics.instrumentation.jvm.JvmMetrics;
import io.prometheus.metrics.model.snapshots.Unit;

import java.io.IOException;
import java.math.BigDecimal;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

// https://prometheus.github.io/client_java/getting-started/metric-types/
public class App {
    public static Gauge temperature;
    public static Histogram requestLatency;
    public static int index = 0;

    public static void t(int thread_num) throws InterruptedException{
        ExecutorService fixedThreadPool = Executors.newFixedThreadPool(thread_num);
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                Timer requestTimer = requestLatency.labelValues("create").startTimer();
                Timer requestTimer2 = requestLatency.labelValues("drop").startTimer();
                try {
                    // System.out.println(Thread.currentThread().getName() + " 开始工作！");
                    Thread.sleep((long) ( Math.random()*2000));
                    requestTimer2.observeDuration();

                    Thread.sleep((long) ( Math.random()*1000));

                    double bj = Math.random()*20.0;
                    double hb = Math.random()*20.0 - 10;
                    BigDecimal b = new BigDecimal(bj);
                    bj = b.setScale(2,BigDecimal.ROUND_HALF_UP).doubleValue();
                    BigDecimal h = new BigDecimal(hb);
                    hb = h.setScale(2,BigDecimal.ROUND_HALF_UP).doubleValue();

                    temperature.labelValues("Beijing").set(bj);
                    temperature.labelValues("Harbin").set(hb);

                    System.out.println(Thread.currentThread().getName() + " index: "+index+", Beijing: "+bj+", Harbin: "+hb);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                finally {
                    requestTimer.observeDuration();
                }
            }
        };
        for (int i = 0; i < 6000; i++) {
            index = i;
            Thread.sleep(2000);
            fixedThreadPool.execute(runnable);
        }
        //Thread.sleep(6000);
        fixedThreadPool.shutdown();
        System.out.println("主线程结束！");
    }
    public static void main(String[] args) throws InterruptedException, IOException {
        requestLatency = Histogram.builder().name("requests_latency_seconds").help("Request latency in seconds.")
                .unit(Unit.SECONDS).labelNames("sqltype").register();

        JvmMetrics.builder().register(); // initialize the out-of-the-box JVM metrics
        Counter counter = Counter.builder()
                .name("my_count_total")
                .help("example counter")
                .labelNames("status")
                .register();

        counter.labelValues("ok").inc();
        counter.labelValues("ok").inc();
        counter.labelValues("error").inc();
        HTTPServer server = HTTPServer.builder()
                .port(9400)
                .buildAndStart();

        temperature = Gauge.builder()
                .name("temperature_celsius")
                .help("current temperature")
                .labelNames("location")
                .unit(Unit.CELSIUS)
                .register();

            t(2);
        temperature.labelValues("Harbin").set(-22.3);

        counter.labelValues("ok").inc();
        temperature.labelValues("Beijing").set(-4.5);


        System.out.println("HTTPServer listening on port http://localhost:" + server.getPort() + "/metrics");

        Thread.currentThread().join(); // sleep forever
    }
}