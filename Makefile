mainDir=cmd/snowflake
packageDir=package
target=snowflake

default: clear prepare compile-osx compile-linux
	@ echo done

compile-osx:
	go build -o ${packageDir}/osx/${target} ${mainDir}/*

compile-linux:
	GOOS=linux GOARCH=amd64 go build -o ${packageDir}/linux/${target} ${mainDir}/*

prepare:
	@ - mkdir -p ${packageDir}/osx
	@ - mkdir -p ${packageDir}/linux

protoc:
	protoc -I . --go_out=plugins=grpc:rpc rpc.proto

clear:
	rm -fr 