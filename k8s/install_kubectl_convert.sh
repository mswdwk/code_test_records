curl -LO  "https://dl.k8s.io/release/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl-convert"

curl -LO "https://dl.k8s.io/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl-convert.sha256"

echo "$(<kubectl-convert.sha256) kubectl-convert" | sha256sum --check

if [ $? -ne 0 ];then
	echo "check failed"
	exit 0
fi

sudo install -o root -g root -m 0755 kubectl-convert /usr/local/bin/kubectl-convert

kubectl convert --help

