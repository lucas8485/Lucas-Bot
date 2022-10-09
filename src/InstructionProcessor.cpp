/*
 *  This file is part of Lucas' Bot.
 *
 *  Lucas' Bot is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License,
 *  or (at your option) any later version.

    Lucas' Bot is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
    for more details.

    You should have received a copy of the GNU Affero General Public License along
    with Lucas' Bot. If not, see <https://www.gnu.org/licenses/>.
 */

#include <MiraiCP.hpp>
#include "InstructionProcessor.h"
#include "PersonCollection.h"
#include <http.h>
#include <regex>
#include <RandomLibrary.h>
#include <windef.h>

/**
 * @brief 获取临时文件名
 * @param suffix 文件后缀
 * @return 包含临时文件名的字符串
 */
std::string getTemporaryFileName(const std::string &suffix) {
    char s[1024];
    // Windows 系统逻辑
#ifdef WIN32
    // 获得临时文件的目录
    FILE *fp = _popen("echo %temp%", "r");
    fscanf(fp, "%s", s);
    _pclose(fp);
    // 将临时文件名附加在目录上，然后返回
    sprintf(s, "%s\\%d.%s", s,  RandomLibrary::getNextRandomNumber(
            RandomLibrary::RandomNumberType::UniformDistribution,
            0, RAND_MAX), suffix.c_str());
#else
    // 类UNIX系统逻辑，类似于Windows系统
    FILE *fp = popen("echo $TMPDIR", "r");
    fscanf(fp, "%s", s);
    pclose(fp);
    sprintf(s, "%s/%d.%s", s, RandomLibrary::getNextRandomNumber(
            RandomLibrary::RandomNumberType::UniformDistribution,
            0, RAND_MAX), suffix.c_str());
#endif
    return std::string{s};
}

void InstructionProcessor::repeat(const MessageDispatcher::SingleInstruction &instruction) {
    // 获得消息来源所在QQ群
    auto group = instruction.event.group;
    // 如果参数为空，则提醒用户输入参数
    if (instruction.arguments.empty()) {
        group.quoteAndSendMessage(MiraiCP::PlainText("复读什么呢？"),
                                  instruction.event.message.source.value());
    } else {
        // 连接参数vector中所有字符串为长字符串
        // 使用空格分隔
        std::string message;
        for (const auto &argument : instruction.arguments) {
            message += argument;
            message += " ";
        }
        // 去除末尾空格
        message.pop_back();
        // 复读
        group.sendMessage(MiraiCP::PlainText(message));
    }
}

void InstructionProcessor::commercialAccount(const MessageDispatcher::SingleInstruction &instruction) {
    char s[1024];
    auto group = instruction.event.group;
    if (instruction.arguments.empty()) {
        group.sendMessage(MiraiCP::PlainText("请输入事物"));
    } else {
        switch (instruction.arguments.size()) {
            case 1:
                sprintf(s, "大家好，这里是琪露诺看世界，今天小编带大家了解%s。%s是怎么一回事？"
                           "%s是什么梗？%s最近为什么这么火呢？\n"
                           "　　大家都知道，%s最近很火，究竟是为什么很火呢？%s到底是什么梗？"
                           "相信大家对%s都很熟悉，%s就是我们每天都会经常遇到的，但是%s是怎么回事呢？"
                           "今天小编就带大家来看看究竟是怎么一回事。\n"
                           "　　%s最近能火，其实就是%s受到了大家的关注。大家可能会感到很惊讶，%s为什么是这样的？"
                           "%s究竟为什么火起来了呢？但事实就是这样，小编也感到非常惊讶。\n"
                           "　　希望小编精心整理的这篇内容能够帮助到你，本期教学结束了，希望这篇文章能对你有所帮助，大家有什么想法，可以在下方评论区给小编留言。",
                        instruction.arguments[0].c_str(), instruction.arguments[0].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[0].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[0].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[0].c_str(), instruction.arguments[0].c_str(),
                        instruction.arguments[0].c_str(),instruction.arguments[0].c_str(), instruction.arguments[0].c_str(),
                        instruction.arguments[0].c_str());
                group.sendMessage(MiraiCP::PlainText(std::string{s}));
                break;
            case 2:
                sprintf(s, "　　大家好，这里是琪露诺看世界，今天小编带大家了解%s。%s到底是怎么一回事呢？让我们一起来看看吧。\n"
                           "　　说起%s，相信大家一定很熟悉，但%s是什么呢？就让小编带大家一起了解吧。\n"
                           "　　其实，%s就是%s，"
                           "大家可能会惊讶，%s竟然就是%s！\n"
                           "　　这就是今天关于%s的全部内容了，不知道大家有什么想法呢？欢迎在屏幕下方留言哦",
                        instruction.arguments[0].c_str(), instruction.arguments[0].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[1].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[1].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[1].c_str(),
                        instruction.arguments[0].c_str());
                group.sendMessage(MiraiCP::PlainText(std::string{s}));
                break;
            case 3:
                sprintf(s, "  大家好，这里是琪露诺看世界，今天小编带大家了解%s%s，"
                           "%s%s是怎么回事呢？"
                           "%s相信大家都很熟悉，但是%s%s是怎么回事呢，下面就让小编带大家一起了解吧。\n"
                           "　%s%s，其实就是%s，"
                           "大家可能会很惊讶%s怎么会%s？但事实就是这样，小编也感到非常惊讶。\n"
                           "　这就是关于%s%s的事情了，大家有什么想法呢，欢迎在评论区告诉小编一起讨论哦！",
                        instruction.arguments[0].c_str(), instruction.arguments[1].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[1].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[0].c_str(), instruction.arguments[1].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[1].c_str(), instruction.arguments[2].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[1].c_str(),
                        instruction.arguments[0].c_str(), instruction.arguments[1].c_str());
                group.sendMessage(MiraiCP::PlainText(std::string{s}));
                break;
            default:
                group.sendMessage(MiraiCP::PlainText("参数过多\n"
                                                     "请输入 [.菜单] 查看指令用法"));
        }
    }
}

void InstructionProcessor::uselessParagraph(const MessageDispatcher::SingleInstruction &instruction) {
    auto group = instruction.event.group;
    if (instruction.arguments.empty()) {
        group.sendMessage(MiraiCP::PlainText{std::string{"请输入主题"}});
        return;
    }
    const auto& topic = instruction.arguments[0];
    std::string url = "https://api.edwiv.com/bzsy/gen.php?event=" + topic +"&length=800&counterType=0&isWritten=0";
    Http::Request request;
    request.setVerbose(true);
    request.setUrl(url);
    request.send();
    auto response = request.getResponse();
    std::regex reg("<[^>]*>");
    response = std::regex_replace(response, reg, "");
    group.sendMessage( MiraiCP::PlainText(response));
}

void InstructionProcessor::trash(const MessageDispatcher::SingleInstruction &instruction) {
    auto group = instruction.event.group;
    static std::vector<std::string> trashes = {
            "差不多得了[流汗][流汗]这么大的事都不拐上原神，原神一招你惹你了，二干了伤天害理的事情，都把你这样了你还不无脑抹黑，米哈游每天费尽心思的文化让路送出中国文化，你这种米孝子只会在网上敲键盘包庇无良公司，中国游戏的未来就是被你这种人毁掉的[流汗][流汗]",

            "原神为什么是神？在谈论这个问题之前，我想先说说其他开放性世界游戏相较于原神究竟差在了哪里\n"
            "　　首先是犯下傲慢之罪的塞尔达传说\n"
            "　　塞尔达传说只是靠着一记mc评分（开放世界游戏评分）取得第一就开始沾沾自喜，这种傲慢的游戏注定走不长远，事实也是如此，靠着一个阴险的mc评分赢了神之后一直在走上坡路，最终泯然众人。与之相比神就很谦卑，不做买断制的都是神留给凡人的慈悲，神不是不会，只是不想通过太完美的表现让玩家绝望所以故意做，可笑有些人不理解神的良苦用心，竟然还用这些事来讥讽神，我劝你们好自为之。\n"
            "　　接着是犯下懒惰之罪荒野大镖客2\n"
            "　　自以为拿下mc评分前20就了不起了。另外大家有所不知，其实在进行评比的时候，r星的人闭上眼睛做冥想的时候，其脑海中看见的画面，正是站在光芒之中的原神，那时神告诉他：“你只可到这里，不可越过（指手机游戏榜）。”然而，神的劝说不但没有让荒野大镖客迷途知返，竟然还敢在mc评分里挤出原神的位置伙同正面挑战神的威严，于是神降下了他的惩罚，荒野大镖客在评分身败名裂，手机玩家没人记得，从此成为了众人口中的大表哥",

            "差不多得了楼主，非抓住uzi没有s冠来黑，他的季后赛亚运会冠军直接无视是吧？人家任天堂都承认原神是原创了，还在这里揪着不放。鸿蒙除了不能用谷歌商店可以说哪里都比ios和安卓好用，就ios那个充电速度和像素别碰瓷mate系列了。丁真他至少带动家乡发展了，人家还是国企员工，你个键盘侠干什么了？\n"
            "抛开事实不谈，明明是女人生的孩子凭什么要随父姓？想随父姓你至少得结扎啊，古圣拿那么多钱肯定是她有实力的，太好看了冰墩墩，王源抽烟都已经道歉了你还在说什么，小黑子能不能闭嘴?你凭什么诋毁嘉然，人家taffy不搞rsq你连认识她的机会都没有，他都是汉利安超人了被20mm口径的钢管暴打怎么就有问题了？\n"
            "我看你就是五十万，你还不感恩自己有饭吃是吧?知不知道59a的重量，没有造过粉药的东西，承认自己是个雏草姬的事实很难是吧?天天就懂得米线米线，你见过凌晨四点的深圳吗?九九六有什么不好的，再努力一点你就可以再努力一点了，这就是电子厂给我的骄傲的资本 我介绍一些有意义的image_emoticon24 丁真为什么抽电子烟\n"
            "①理塘海拔高，传统香烟燃烧慢，烟雾小，使用口感较差\n"
            "②当地经济落后，传统香烟易造成当地居民资金不足问题\n"
            "③传统与变革碰撞，随着科技和经济发展，电子烟越来越受年轻人青睐\n"
            "④电子烟属于科技产品，能带动理塘高新技术产业发展\n"
            "⑤因此，丁真爱抽电子烟 王源为什么抽传统香烟\n"
            "①重庆市位于四川盆地边缘，海拔低，空气较稠密，利于传统香烟燃烧\n"
            "②重庆市位于亚热带季风气候区，夏季多洪涝灾害，易停电，电子香烟无法保证夏季续航\n"
            "③重庆多丘陵，地形崎岖，利于种植烟草\n"
            "④重庆市60岁以上人口约占重庆市总人口的22%，人口老龄化严重，当地人思想较保守，所以当地人爱抽传统香烟\n"
            "⑤因此，王源爱抽传统香烟丁真为什么抽电子烟\n"
            "①理塘海拔高，传统香烟燃烧慢，烟雾小，使用口感较差\n"
            "②当地经济落后，传统香烟易造成当地居民资金不足问题\n"
            "③传统与变革碰撞，随着科技和经济发展，电子烟越来越受年轻人青睐\n"
            "④电子烟属于科技产品，能带动理塘高新技术产业发展\n"
            "⑤因此，丁真爱抽电子烟 王源为什么抽传统香烟 ①重庆市位于四川盆地边缘，海拔低，空气较稠密，利于传统香烟燃烧\n"
            "②重庆市位于亚热带季风气候区，夏季多洪涝灾害，易停电，电子香烟无法保证夏季续航\n"
            "③重庆多丘陵，地形崎岖，利于种植烟草\n"
            "④重庆市60岁以上人口约占重庆市总人口的22%，人口老龄化严重，当地人思想较保守，所以当地人爱抽传统香烟\n"
            "⑤因此，王源爱抽传统香烟",

            "上次在公交车爱心座上玩Phigros，突然听到有人叫我臀批，因为我是c语言大佬，所以我直接用程序黑了他的手机，他发现他手机被黑后气急败坏，顺手拿起一根20cm粗的钢管打我，但我由于打小就挨揍，抗性直接拉满，他打了我半个小时，把钢管硬生生打断了，只有点皮外伤，然后我把他打得青一块紫一块的，让他住了好几天院，后来他出院了，给我赔礼道歉，请我喝了杯白水，我一喝，好家伙，原来是硫酸，那钻心的疼让我一生难忘，然后我看到前面有一团红色，以为是鞭炮，准备吓一吓他，好家伙，原来是核弹，还好我和他都是体育生，跑得快，腿脚还利落，只是手被炸断了，在医院，医生测量了下我的体温，好家伙一百多度，本来是不疼的，结果庸医误诊说修养就好了，然后我拖着断手过了一周，疼得受不了了，才去复诊，打了仨月石膏。。。然而我现在还躺在病床上，令人感叹",
            "你说的对，但是《理论值》是由005自主研发的一款科幻游戏。游戏发生在一个被称作「Arcaea」的幻想世界，在这里，被void和星熊南巫选中的人将被授予「ipad pro2018」，导引616之力。你将扮演一位名为「洞洞五」的神秘角色，在自由的旅行中拯救性格各异、能力独特的歌曲们，和他们一起达成理论值，找回失散的鸡蛋饼。成长为两周炸船、五天杀半球、十六天杀BB的音游大神，并消灭里SIGMA、亚斯塔禄等邪恶份子——同时，逐步发掘「Testify Beyond难度」的真相，以此击溃韵律源点的阴谋，夺回疯狂星期四的食用权。",


            "你说的对，但是《Phigros》是由米哈游自主研发的一款全新开放世界冒险音乐游戏。游戏发生在一个被称作「Arcaea」的幻想世界，在这里，被凯尔希选中的人将被授予「荣耀王者」，导引三相之力。你将扮演一位名为「生日快乐是个呆瓜」的神秘角色，在自由的旅行中拯救性格各异、能力独特的英灵们，和他们一起击败整合运动，找回失散的ek鲁比——同时，逐步发掘「水滴」的真相。《Phigros》是由米哈游同哔哩哔哩合作研发的一款全新开放世界冒险音乐rpg游戏。游戏发生在一个被称作「Arcaea」的出生世界，在这里，被嘉然选中的人将被授予「逼三狼」，导引宗师之力。你将扮演一位名为「生日快乐是个呆瓜」的神秘角色，在危机合约中拯救性格各异、能力独特的英灵们，和他们一起击败名为otto的魔王，找回失散的ek鲁比——同时，逐步发掘「三国杀」的真相，游戏拥有强大的枪械系统，高度自由的3d沙盒活动，以及抽卡养成玩法，凯尔希、美羊羊、东雪莲等强力角色等你获得，更有屠龙宝刀一刀999，金色大翅膀+99。游戏采用全新元宇宙玩法，支持vr、3d以及二次元画风，期待玩家同我们一起蒸蒸日上。你说的对，但是《Phigros》是由米哈游自主研发的一款全新开放世界冒险音乐游戏。游戏发生在一个被称作「Arcaea」的幻想世界，在这里，被凯尔希选中的人将被授予「荣耀王者」，导引三相之力。你将扮演一位名为「生日快乐是个呆瓜」的神秘角色，在自由的旅行中拯救性格各异、能力独特的英灵们，和他们一起击败整合运动，找回失散的ek鲁比——同时，逐步发掘「水滴」的真相。而且我是个楠酮你说的对，但是《Phigros》是由米哈游自主研发的一款全新开放世界冒险音乐游戏。游戏发生在一个被称作「Arcaea」的幻想世界，在这里，被凯尔希选中的人将被授予「荣耀王者」，导引三相之力。你将扮演一位名为「生日快乐是个呆瓜」的神秘角色，在自由的旅行中拯救性格各异、能力独特的英灵们，和他们一起击败整合运动，找回失散的ek鲁比——同时，逐步发掘「水滴」的真相。你说的对，所以你愿意来玩Phigros吗？《Phigros》是由米哈游和peroperogames一起研发的一款全新开放世界冒险音乐游戏。游戏发生在一个被称作「Arcaea」的幻想世界，在这里，被神选中的人将被授予「旅行者徽章」，导引音矿之力。你将扮演一位名为「乙姬」的神秘角色，在自由的旅行中邂逅性格各异、能力独特的领航员们，和她们一起击败中二企鹅，找回失散的Ivy——同时，逐步发掘「奏坂学院」的真相。尊敬的各位b友、各位初升、各位[星瞳_发癫][永雏塔菲_亲嘴]和UP主的粉丝：\n"
            "今天我们怀着奥利安费的心情，来恭喜@_Karasu_ 电棍：国玉的战争在2022年8月2日晚8点53分，达到了1000w播放量。在此，我衷心地感谢大家，在百忙之中来到这里，与我们共同分担这份喜悦。\n"
            "祝你越走越远，电棍：国玉的战我们永远喜欢你！\n"
            "最后，让我们大声的喊出那句传世经典:尊尼↑获↓加是你↑爸↑↓↑\n"
            "同时也祝贺三位U卩(三拖四)主受到古神哈姆的保佑，事业也会越来越好！",

            "请复制粘贴下去\n"
            "继续支持原神。\n"
            "敌人的目的是把原神搞垮掉，我们就是要帮原神顶起来。\n"
            "它们想通过举报让米哈游被迫改衣服，从而让玩家生气上头，引起和mhy的对立甚至退坑。\n"
            "可我就不退坑，我还要声援。哪怕米哈游所有角色都裹上大棉袄变成隐形人，我也逼着自己玩下去。\n"
            "不管我们要面对的敌人是谁，我们的格局不应该只限制在一个游戏上，我们要看到这场风波背后那些人。\n"
            "往小了说是**搞鬼，往中了说是tx网易，往大了说，就是内鬼在______\n"
            "云堇在国际影响力多大？关注外网的都知道，为啥这节奏早不来晚不来，偏偏2.4的时候来？\n"
            "为啥唐氏少年这种垃圾还能被力推？\n"
            "大家好好想想吧，目前玩家内讧加冲米哈游是最蠢的事情，我们自己力量都分散，怎么斗得过潜在的在暗处的那些牛鬼蛇神？\n"
            "团结，团结，团结！继续支持原神，支持原神文化输出，支持我们的戏曲文化通过游戏的方式向全世界宣传。\n"
            "相信有人能够处理好现在的局面。\n"
            "认清敌人，坚定信心！\n"
            "还有，文化战已经开始了，其残酷程度不亚于真实战争，我们每个人都在这场漩涡里，没法独善其身，同时我们每个人也都是斗士，原神只是开始，往后还会有更多文化产品遭到攻击，能不能打赢这场文化战且看我辈，犯我中华者其远必诛。真的太震撼了，这次心海的传说任务。以前我就认为米哈游的剧情组有世界级的艺术水平，但这次心海的传说任务更是脍炙人口，让人仿佛观看了一场莎士比亚的戏剧，无论是起承转合，还是矛盾冲突，人物塑造，都达到了世界一流大师的水平。尤其是在米哈游极为擅长的人物群像塑造上，心海传说任务更是达到了炉火纯青的地步，无论是堪比诸葛亮的心海，还是作为线索人物的旅行者，乃至天领奉行的九条的塑造都完美的体现并丰满了人物的性格。而他们之间的碰撞冲突也使得这个杰作更具有戏剧性。与此同时，作为心海的传说任务，这次剧情不遗余力的展现了心海强到令人发指的战略能力和统兵能力，体现了她强大的各人魅力，真的很荣幸能看到这么完美的一次传说任务，真的感动。在这个浮躁的社会，一个不玩原神的人是活的很失败的，不分男女。\n"
            "玩高质量的手游，是人类从二十一世纪以来便存在的行为，而玩原神，体现了一个人的自身价值和身上的附加价值，玩原神，是对自己的一种肯定。\n"
            "一个不玩原神的人，有两种可能性。一种是没有能力玩原神，买不起高配的手机和抽不起卡等各种自身因素，一个没有能力玩原神的人，无论是因为自身相貌还是经济实力、社会地位种种因素不支持，那么，他的人生就是失败的，是灰暗的，是不被真正的上流社会和高端人士认可的，试问，如果一个人连手机都买不起，氪金抽卡都做不到的话，他的存在还有什么意义？当你的自身条件足够优秀，有的是大把的钱财买高端的手机，抽各种想要的人物，游戏厂商也有做出新内容的动力，不仅促进了玩家和厂商之间双赢的局面，也拉动了GDP，推动了经济发展，这是一个多方收益的局面。\n"
            "可见，玩原神是一个人证明自己的有力手段，如果连证明自己都做不到，可见人生的失败。\n"
            "第二种可能：有能力却不玩原神的人，一个人，在有能力而没有玩原神的想法时，那么这个人的思想境界便低到了一个令人发指的程度。一个有能力的人不付出行动来证明自己。原神玩家每天都会发成千上万条评论，而且原神都这么出名了，人也多。你就非要挑出来挂最逆天的这几句话吗？还说什么欧泡之类的东西。也就是原神玩家素质高，懒得管也不想管你这个，但凡放在别的游戏圈子里怎么可能容忍你这种挂人的行为？我来说句公道话，请挂人的和跟风黑的积点口德吧。再怎么说原神玩家三观是要比某些人正很多的，为何非要揪着一点黑不放呢？大家加入这场无意义的狂欢只会造成伤害，理智一点，别被别有用心的人带节奏了。#(汗)",

            "贵贴没有任何规定说不能讨论原神 m，我又何罪之有呢，再说你们@了那么多回吧务部结果也没事，我估计吧务部的人大概也玩原神，你们@了那么多回这就显得很尴尬知道吗【图片】【图片】",

            "适可而止矣！涓埃之事，亦央原神。夫原神者，其一未犯尔，其二固道，尔以何素愚辱之？米哈游者，欲彰华夏之文涵于天下，殚精竭虑。尔等竖子，仗键盘跋扈于网路，构陷良企于冤雪，不舍昼夜。华夏娱戏之前景，皆败于尔属",

            "在屁股肉这款音游红火之前，其实还有一款音游，名为arcaea m，虽然远不及屁股肉的影响力，但也是收货了不少的玩家，我也是其中的一员，他公测的那一年，我也创建了我的嗦哩嗦哩账号，并起名为—生日快乐是个呆子#(欢呼) ；之后的一段时间我不停的上传我的理论值以及单手打歌的arc视频，收货了一大批的粉丝，我也被粉丝们称为—呆神，每次别的up主发音游视频，都会有我的粉丝在下面说——感觉不如呆神#(太开心) 。\n我的嗦哩嗦哩账号也很快的到达了一伯万粉丝，成为了音游届的超级音游王，在我辉煌之际，却不断的有噩耗传来…\narc因为两年不更主线，饱受诟病，终于有一位玩原神 m的神秘C语言大佬看不下去了，黑进了616服务器，还盒盒了游戏制作人guy，之后没几天游戏制作人guy还有优秀的arc谱师吐丝被杀人魔—钢管怪人，用20cm粗的钢管残忍沙嗨，到现场时只看到断掉的20cm粗的钢管以及用雪写下的一句话——感觉不如原神#(汗) 。我也因为经常打个月交先，导致我的左手重伤，被迫退役#(喝酒)\n我坐在屯达集公园的长椅上，用颤抖的手登上了我尘封已久的账号——生日快乐是个呆子…",

            "可你不是....可你不是枫原万叶，会替我挡下无想一刀。可你不是派蒙，会一直陪伴我。可你不是荧，令我朝思暮想。可你不是可莉，会带我去炸鱼。可你不是菲谢尔，会陪我一起中二。可你不是行秋，隐藏身份帮助我。可你不是宵宫，会陪我一起看烟花。可你不是达达利亚，在千岩军手下救我。可你不是托马，在我危难时刻救我一命。可你不是荒泷一斗，愿意把我当兄弟。可你不是钟离，给不了我安全感。可你不是影，会走出一心净土来陪我。可你不是丽莎，会叫我小可爱。可你不是胡桃，会在百忙之中逗我笑。可你不是神里绫华，在河流之中为我跳舞"
    };
    // 随机选择一条消息发送
    auto index = RandomLibrary::getNextRandomNumber<decltype(trashes)::size_type>(
            RandomLibrary::RandomNumberType::UniformDistribution,
            0, trashes.size() - 1);
    group.sendMessage(MiraiCP::PlainText{trashes[index]});
}

void InstructionProcessor::sick(const MessageDispatcher::SingleInstruction &instruction) {
    auto group = instruction.event.group;
    if (instruction.arguments.empty()) {
        group.sendMessage(MiraiCP::At(instruction.event.sender.id()),MiraiCP::PlainText{std::string{"请输入对象"}});
        return;
    }
    const auto& object = instruction.arguments[0];
    char s[1024];
    sprintf(s, "来个%s吧 求求了 我已经寡疯了 我真的想%s想发疯了 我躺在床上会想%s我洗澡会想%s 我出门会想%s "
               "我盯着路边的%s看 我盯着地铁的%s看 我盯着好朋友的%s看 "
               "我好孤独啊 这世界那么多%s为什么就没有一个%s属于我的呜呜呜呜", object.c_str(), object.c_str(), object.c_str(), object.c_str(), object.c_str(),
            object.c_str(), object.c_str(), object.c_str(), object.c_str(), object.c_str());
    group.sendMessage(MiraiCP::PlainText{std::string{s}});
}

void InstructionProcessor::kfcCrazyThursday(const MessageDispatcher::SingleInstruction &instruction) {
    auto group = instruction.event.group;
    Http::Request req;
    req.setUrl("http://thursday.gouyin.net/api/kfc.php");
    req.setVerbose(true);
    req.send();
    auto res = req.getResponse();
    // Parse res as JSON
    // Response type is:
    // [
    //   {
    //     "id": 0,
    //     "content": "string"
    //   }
    // ]
    auto json = nlohmann::json::parse(res);
    group.sendMessage(MiraiCP::PlainText{std::string{json[0]["content"]}});
}

void InstructionProcessor::touhouProject(const MessageDispatcher::SingleInstruction &instruction) {
    auto group = instruction.event.group;
    Http::Request req;
    req.setUrl("https://img.paulzzh.tech/touhou/random");
    req.setVerbose(true);
    req.send();
    auto response = req.getResponse();

    std::string filename = getTemporaryFileName("jpg");
    Http::FileUtilities::writeFile(filename, response);

    auto uploadedImage = group.uploadImg(filename);
    group.sendMessage(uploadedImage);
}

void InstructionProcessor::signOn(const MessageDispatcher::SingleInstruction &instruction) {
    auto group = instruction.event.group;
    // 根据用户QQ号，在数据库中查找用户
    auto& person = PersonCollection::getPerson(instruction.event.from()->id());
    // 检查用户是否签到
    if (person.isSignOnToday()) {
        group.sendMessage(MiraiCP::At(instruction.event.sender.id()),MiraiCP::PlainText("你今天已经签到过了"));
    } else {
        // 获得之前的铜钱数
        auto previousCopperCoins = person.getCopperCoin();
        // 签到
        person.signOn();
        // 获得现在的铜钱数
        auto currentCopperCoins = person.getCopperCoin();
        // 计算获得的铜钱数
        auto copperCoinsDifference = currentCopperCoins - previousCopperCoins;

        char s[1024];
        sprintf(s, "签到成功！\n"
                   "你获得了 %d 个铜钱\n"
                   "你现在有 %d 个铜钱\n"
                   "你已经连续签到 %d 天", copperCoinsDifference, currentCopperCoins, person.continuousSignOnDays());
        group.sendMessage(MiraiCP::At(instruction.event.sender.id()), MiraiCP::PlainText(s));
    }
}

void InstructionProcessor::otherMessage(const MessageDispatcher::SingleInstruction &instruction) {
    // 检查这个人是否需要被攻击
    auto& person = PersonCollection::getPerson(instruction.event.from()->id());
    if (person.willBeAttackNextTime()) {
        // 怼
        auto group = instruction.event.group;
        group.quoteAndSendMessage( MiraiCP::PlainText("鳖载着理发店"),
                                   instruction.event.message.source.value());
        // 重置计数
        person.resetAttackCount();
    } else {
        // 计数
        person.increaseSpeakCount();
    }
}

void InstructionProcessor::menu(const MessageDispatcher::SingleInstruction &instruction) {
    auto group = instruction.event.group;
    group.sendMessage(MiraiCP::PlainText(std::string{"菜单：\n"
                                         "1.复读\n"
                                         "使用方法：\n"
                                         ".复读 [内容]\n"
                                         "2.营销号\n"
                                         "使用方法：\n"
                                         ".营销号 [事物]\n"
                                         ".营销号 [事物] [别称]\n"
                                         ".营销号 [事物] [事件] [结果]\n"
                                         "3.说废话\n"
                                         "使用方法：\n"
                                         ".废话 [主题]\n"
                                         "4.发病\n"
                                         "使用方法：\n"
                                         ".发病 [主题]\n"
                                         "5.倒垃圾\n"
                                         "使用方法：\n"
                                         ".倒垃圾\n"
                                         "6.疯狂星期四V我50\n"
                                         "使用方法：\n"
                                         ".kfc\n"
                                         "7.东方Project图片生成\n"
                                         "使用方法：\n"
                                         ".东方\n"
                                         ".车万\n"
                                         "8.签到\n"
                                         "使用方法：\n"
                                         ".签到\n"
                                         ".打卡\n"
                                         "9.钓鱼 [开发中]\n"
                                         "使用方法：\n"
                                         ".钓鱼\n"
                                         "10.打工 [开发中]\n"
                                         "使用方法：\n"
                                         ".打工\n"
                                         "11.五千兆表情包生成 [开发中]\n"
                                         "使用方法：\n"
                                         ".五千兆 [上排文字] [下排文字]\n"
                                         "12.菜单"
                                         "使用方法：\n"
                                         ".菜单\n"
                                         "13.关于\n"
                                         "使用方法：\n"
                                         ".关于\n"}));
}

void InstructionProcessor::about(const MessageDispatcher::SingleInstruction &instruction) {
    auto group = instruction.event.group;
    group.sendMessage(MiraiCP::PlainText(std::string{"作者：Lucas\n"
                                                     "项目地址：\n"
                                                     "https://gitme.fun/lucas8485/Lucas-Bot\n"
                                                     "遵循AGPLv3协议开源\n"
                                                     "使用的第三方库：\n"
                                                     "MiraiCP\n"
                                                     "nlohmann/json\n"
                                                     "libcurl\n"
                                                     "utf8-for-cpp"}));

}

