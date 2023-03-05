# openjson
跨平台多线程设计
OpenJson是最好用的高性能C++json解析器，非常简单易用，解析速度超快，可以解析超过1GB以上的json文件。

**OpenLinyou项目设计跨平台服务器框架，在VS或者XCode上写代码，无需任何改动就可以编译运行在Linux上，甚至是安卓和iOS.**
OpenLinyou：https://github.com/openlinyou

## 运行环境
Windows、linux、Mac、iOS、Android等跨平台设计

## 编译和运行
请安装cmake工具，用cmake可以构建出VS或者XCode工程，就可以在vs或者xcode上编译运行。
源代码：https://github.com/openlinyou/openjson
```
git clone https://github.com/openlinyou/openjson
cd ./openjson
mkdir build
cd build
cmake ..
#如果是win32，在该目录出现openjson.sln，点击它就可以启动vs写代码调试
make
./test
```

## 全部源文件
+ src/openjson.h
+ src/openjson.cpp


## 测试例子
```C++
#include "openjson.h"
#include <iostream>
#include <assert.h>
int main()
{
    std::string buff1;
    //生成json字符串
    {
        OpenJson json;
        auto& nodeA = json["a"];
        nodeA["aa"] = "aa12";
        nodeA["ab"] = 123;
        auto& nodeB = json["b"];
        //迭代数组原生
        for (size_t i = 0; i < 2; i++)
        {
            auto& node = nodeB[i];
            if (i == 0)
            {
                node["ba"]["key1"] = std::string("value_ba");
            }
            else
            {
                uint64_t val  = 999999999999999;
                node["bb"][0] = val;
                node["bb"][1] = 1.3;
            }
        }
        //设置值
        json["b"][1]["bb"][2] = true;
        buff1 = json.encode();
    }
    std::string buff2 = "{"
        "\"a\":{"
            "\"aa\":\"aa12\","
            "\"ab\":123"
        "},"
        "\"b\":["
            "{\"ba\":{\"key1\":\"value_ba\"}},"
            "{\"bb\":[999999999999999,1.3,true]}"
        "]}";

    assert(buff1 == buff2);
    //解析json字符串
    {
        OpenJson json;
        json.decode(buff2);
        auto& nodeA = json["a"];
        assert(nodeA["aa"].s() == "aa12");
        assert(nodeA["ab"].i32() == 123);
        auto& nodeB = json["b"];
        for (size_t i = 0; i < nodeB.size(); i++)
        {
            auto& node = nodeB[i];
            if (i == 0)
            {
                assert(node["ba"]["key1"].s() == "value_ba");
            }
            else
            {
                assert(node["bb"][0].i64() == 999999999999999);
                assert(node["bb"][1].d() == 1.3);
            }
        }
        assert(json["b"][1]["bb"][2].b() == true);
        //删除元素
        json["b"][1]["bb"].remove(1);
        json.encodeFile("./test.json");
    }
    buff2 = "{"
        "\"a\":{"
        "\"aa\":\"aa12\","
        "\"ab\":123"
        "},"
        "\"b\":["
        "{\"ba\":{\"key1\":\"value_ba\"}},"
        "{\"bb\":[999999999999999,true]}"
        "]}";
    {
        //加载json文件
        OpenJson json;
        json.decodeFile("./test.json");
        buff1 = json.encode();
        assert(buff1 == buff2);
    }
    buff2 = "{\"b\":[{\"bb\":[99999]}]}";
    {
        OpenJson json;
        json["b"][1]["bb"][2] = 99999;
        buff1 = json.encode();
        assert(buff1 == buff2);
    }

    {
        OpenJson json1;
        json1["a"] = "[\"abc\"]";
        buff2 = json1.encode();

        buff1 = "{\"a\":\"[\\\"abc\\\"]\"}";
        assert(buff2 == buff1);

        OpenJson json2;
        json2.decode(buff2);
        assert(buff2 == json2.encode());
    }
    std::cout << "公众号:https://mp.weixin.qq.com/s/doD_JNm3rNBqQIOrscOnqw" << std::endl;
    return 0;
}
```


