# PA-CW2

## Run tests

```
$(docker images opencilk | grep v1.1 -q) && echo "Already installed" || $(cd $(mktemp -d) && wget https://github.com/OpenCilk/opencilk-project/releases/download/opencilk%2Fv1.1/docker-opencilk-v1.1.tar.gz && docker load < docker-opencilk-v1.1.tar.gz)
docker run --rm -v{PROJECT_DIR}:/root/CW2 -ti opencilk:v1.1 sh -c 'apt-get install -y git wget && \
wget https://raw.githubusercontent.com/deepsea-inria/pctl/master/script/get.sh && \
chmod +x get.sh && \
./get.sh && \
cd /root/CW2/build && make && \
taskset --cpu-list 0-8:2 ./a.out"
```

## Possible output
```
19038
10048
18226
8735
18215
8669
18201
8453
18597
8719
19038 18226 18215 18201 18597 18455
10048 8735  8669  8453  8719  8924
```
First long line -- ms in seq version of algo (last value -- average)

Last  long line -- ms in par version of algo (last value -- average)
