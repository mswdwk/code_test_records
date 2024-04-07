
go install github.com/golangci/golangci-lint/cmd/golangci-lint@v1.57.2
NAME=`uname -n`
if [ "$NAME" == "Arch" ];then
	yay -S yamllint
	yay -S yamlfmt
fi
