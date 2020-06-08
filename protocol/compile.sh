#!/bin/bash

protoc -I ./ --cpp_out=. qigrpc.proto
protoc -I ./ --grpc_out=. --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin qigrpc.proto