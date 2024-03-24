package main

import (
	"net/http"

	"github.com/prometheus/client_golang/prometheus"
	"github.com/prometheus/client_golang/prometheus/promhttp"
	// "github.com/prometheus/client_golang/prometheus/promhttp/cookie"
)

/*func main() {
	counter := prometheus.NewCounter(
		prometheus.CounterOpts{
			Name: "example_counter",
			Help: "An example counter",
		},
	)
	prometheus.MustRegister(counter)
	// 创建一个使用cookie-based认证的Handler
	handler := cookie.NewHandler(&cookie.Config{
		Username: "user",
		Password: "pass",
		Realm:    "metrics",
	})
	// 将Prometheus的handler设置到handler中
	handler.SetHandler(promhttp.Handler().ServeHTTP)
	http.Handle("/metrics", handler)
	log.Fatal(http.ListenAndServe(":8080", nil))
}*/

// 结合prometheus一起使用
func main() {
	/*
	   自定义registry(默认prometheus注册的registry不适用时)
	*/

	counter := prometheus.NewCounter(
		prometheus.CounterOpts{
			Name: "example_counter",
			Help: "An example counter",
		},
	)
	registry := prometheus.NewRegistry()

	// 注册指标
	registry.MustRegister(counter)
	counter.Add(1)

	http.Handle("/metrics", baseAuth(promhttp.HandlerFor(registry, promhttp.HandlerOpts{Registry: registry})))
	http.ListenAndServe(":8050", nil)
}

/*中间件密码认证*/
func baseAuth(h http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		username, password, ok := r.BasicAuth()
		// fmt.Println(username, password, "----->")
		if !ok {
			AuthFailed(w, "401 Unauthorized!")
			return
		}
		if username != "admin" || password != "admin123" {
			AuthFailed(w, "401 Password error!")
			return
		}
		h.ServeHTTP(w, r)
	})
}

/*认证失败*/
func AuthFailed(w http.ResponseWriter, msg string) {
	w.Header().Set("WWW-Authenticate", `Basic realm="My METRICS"`)
	http.Error(w, msg, http.StatusUnauthorized)
}
