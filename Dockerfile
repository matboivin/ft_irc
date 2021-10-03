#Dockerfile for CI/CD image

#we are using ubuntu base image
FROM ubuntu:18.04
# installing requirements to get and extract prebuilt binaries
RUN apt-get update && apt-get install -y build-essential\
 xz-utils \
 curl python3 \
 && rm -rf /var/lib/apt/lists/*
#Getting prebuilt binary from llvm 

RUN if [ "$(uname -m)" = "aarch64" ];then LINK="https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/clang+llvm-10.0.0-aarch64-linux-gnu.tar.xz" ; \
else LINK="https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/clang+llvm-10.0.0-x86_64-linux-gnu-ubuntu-18.04.tar.xz" ; fi ; \ 
curl -SL "$LINK" \
 | tar -xJC . && \
 mv clang* clang_10 

RUN  cp -r ./clang_10/bin/* /bin/. ; cp -r ./clang_10/lib/* /lib/. 
#start bash
CMD [ "/bin/bash" ]
