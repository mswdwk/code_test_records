sudo kubeadm init --apiserver-advertise-address=192.168.79.134 \
--image-repository registry.aliyuncs.com/google_containers \
--kubernetes-version v1.23.2 \
--pod-network-cidr=10.2.0.0/16
