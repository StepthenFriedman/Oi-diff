## Oi - Diff

> 一个 Oi 自用对拍程序,支持检测 WA,TLE,MLE,RE 和速度对比.

<img src="https://img.shields.io/badge/Author-woshiluo-blue.svg?style=flat-square"/>

```
gen.cpp 数据生成器
diff.ccp 对拍程序
mine.cpp 自己的程序
master.cpp 正确的程序
```

安装:
```
chmod 777 ./install.sh
./install.sh
```
使用:
```
oj {build,version,run,check,race,update} [-i input_file] [options]
options:
    -sd         run on specific data (data/gen.in) without data generator.
    -sd=<file>  run on specific data without data generator.
    -t          set time limit (ms). default: 1200.
    -m          set memory limit (KB). default: 2512 (that is 2.4 MB). only work with -vg.
    -mle=<bool> false by default. detecy MLE with valgrind massif. memory statistics may vary from online judge, and the analyzing process will be longer.
    -tle=<bool> true by default. detect TLE. time cost statistics may vary from online judge.
    -wa=<bool>  true by default. detect WA.

oj verbs:
    build       create mine.c/cpp, master.c/cpp, gen.c/cpp according to input files.
    run         compile and run mine.c/cpp. cannot detect WA.
    check       compile and run mine.c/cpp, master.c/cpp, gen.c/cpp and compare result.
    race        compile and run mine.c/cpp, master.c/cpp, gen.c/cpp and compare time cost.

example useage:
oj update
oj build -i myprogram.cpp -i theirprogram.cpp -i generator.cpp
oj check -mle=true
```
源码文件只有diff.c
可以使用 洛谷的`CYaRon`进行更加优秀的数据生成


