# openjson
Cross-platform multi-threaded design!
OpenJson is the world's best high-performance C++json parser

**The OpenLinyou project designs a cross-platform server framework. Write code in VS or XCode and run it on Linux without any changes, even on Android and iOS.**
OpenLinyou：https://github.com/openlinyou

## Cross-platform support
Linux and Android use epoll, iOS and Mac use kqueue, other systems (Windows) use select, so the number of io cannot exceed 64.

## Compilation and execution
Please install the cmake tool. With cmake you can build a VS or XCode project and compile and run it on VS or XCode. 
Source code:https://github.com/openlinyou/openjson
```
# Clone the project
git clone https://github.com/openlinyou/openjson
cd ./openjson
# Create a build project directory
mkdir build
cd build
cmake ..
# If it's win32, openjson.sln will appear in this directory. Click it to start VS for coding and debugging.
make
./test
```

## All source files
+ src/openjson.h
+ src/openjson.cpp


## Test Demo
```C++
#include "openjson.h"
#include <iostream>
#include <assert.h>
int main()
{
    std::string buff1;
    //stringify
    {
        OpenJson json;
        auto& nodeA = json["a"];
        nodeA["aa"] = "aa12";
        nodeA["ab"] = 123;
        auto& nodeB = json["b"];
        //iterate element
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
        //set element
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
    //parse
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
        //remove element
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
    std::cout << "complete!\nhttps://mp.weixin.qq.com/s/doD_JNm3rNBqQIOrscOnqw" << std::endl;
    return 0;
}
```
