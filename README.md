## Oi - Diff

> 一个 Oi 自用对拍程序

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
oj {build,version,run,check,race,update} [-i infile] [options]
options:
    -sd         run on specific data without data generator.
    -t          set time limit (ms). default: 1200.
    -m          set memory limit (KB). default: 2512 (that is 2.4 MB). only work with -vg.
    -vg         enable valgrind massif, support MLE detect. memory statistics may vary from online judge.
    -tle=<bool> true by default. detect TLE.

example:
oj check
oj update
oj check -i myprogram.cpp -i theirprogram.cpp -i generator.cpp -vg
```

可以使用 洛谷的`CYaRon`进行更加优秀的数据生成


