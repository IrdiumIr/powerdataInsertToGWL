// cube_gwl_process.cpp : 对生成的cube_gwl文件进行内容解析处理

#include <iostream>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

int main()
{

    string source_txt = "C:\\VSCodeCpp/cube_gwl.txt";
    string target_txt = "C:\\VSCodeCpp/cube_gwl_new.txt";

    //setting_now 从0变成1时插入add1   从1变成0时插入add2
    //cube体跨越正负边界
    bool setting_now = 0;
    string add1 = "LaserPower $p1\nScanSpeed $s1";
    string add2 = "LaserPower $p2\nScanSpeed $s2";

    string tag = "Write";

    ifstream source(source_txt);
    ofstream target(target_txt , ofstream::trunc);
    //trunc 截断（覆盖文件中原有的内容） 不使用app
    
    if (!source.is_open()) {
        cout << "Open file failed.\nPlease make sure the source file exists." << endl;
        return 0;
    }
    
    string temp;
    //write -（x,y,z）- (x,y,z) - write
    queue<string> lines;

    for (int i = 0; i != 3; ++i)
    {
        if (getline(source, temp))
            lines.push(temp);
        else
        {
            cout << "Please check file to make sure the file is not empty." << endl;
            return 0;
        }
    }

    bool first = 1;
    while (getline(source, temp))
    {

        // if (lines.size() < 3)
        // {
        //     lines.push(temp);
        //     continue;
        // }

        //忽略原有的设置"solid"
        if (temp == "LaserPower $solidLaserPower" || temp == "ScanSpeed $solidScanSpeed")
            continue;

        // 两行被tag"Write"包围  检查两行中的数据  只检查被包围的两行中的第二行的第一个字符
        //  对应文件中line11-16  此时lines={write (-20,-20,0) (-20,20.0)} temp=write
        //  Write
        //  LaserPower $solidLaserPower
        //  ScanSpeed $solidScanSpeed
        //  -20	-20	0
        //  -20	20	0
        //  Write
        if (lines.front() == tag && temp == tag) 
        {

            // 每层第一次初始化
            if (first) 
            {
                first = 0;
                if (lines.back()[0] == '-') 
                {
                    setting_now = 0;
                    target << lines.front() << '\n';
                    lines.pop();
                    target << add1 << '\n';
                    lines.push(temp);
                }
                else 
                {
                    setting_now = 1;
                    target << lines.front() << '\n';
                    lines.pop();
                    target << add2 << '\n';
                    lines.push(temp);
                }
            }

            else if (lines.back()[0] == '-' && setting_now == 1) 
            {
                setting_now = 0;
                target << lines.front() << '\n';
                lines.pop();
                target << add1 << '\n';
                lines.push(temp);
            }

            else if(lines.back()[0] != '-' && setting_now == 0) 
            {
                setting_now = 1;
                target << lines.front() << '\n';
                lines.pop();
                target << add2 << '\n';
                lines.push(temp);
            }

            else 
            {
                target << lines.front() << '\n';
                lines.pop();
                lines.push(temp);
            }
       
        }
        else
        {
            //新的一层
            if (temp.substr(0,7) == "% Slice")
                first = 1;

            //
            target << lines.front() << '\n';
            lines.pop();
            lines.push(temp);
        }

    }

    while(!lines.empty())
    {
        target << lines.front() << '\n';
        lines.pop();
    }
    
    source.close();

    return 0;
}