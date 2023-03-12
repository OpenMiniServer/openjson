#include <assert.h>
#include "openjson.h"
#include <iostream>

using namespace open;

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