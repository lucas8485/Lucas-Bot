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

#ifndef MIRAICPPLUGIN_MESSAGE_DISPATCHER_H
#define MIRAICPPLUGIN_MESSAGE_DISPATCHER_H

#include <MiraiCP.hpp>
#include <string>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>
#include <functional>

namespace MessageDispatcher {
    // 全局类型定义

    /**
     * @brief 指令前缀
     */
    constexpr char instructionPrefix = '.';

    /**
     * @brief 消息类型
     * 对应MiraiCP的文档说明
     */
    enum class MessageType {
        MarketFace = -5,
        OnlineForwardedMessage = -4,
        OnlineAudio = -3,
        QuoteReply = -2,
        UnSupportMessage = -1,
        PlainText = 0,
        At = 1,
        AtAll = 2,
        Image = 3,
        App = 4,
        Service = 5,
        File = 6,
        Face = 7,
        FlashImage = 8,
        MusicShare = 9
    };

    /**
     * @brief 指令类型，根据不同的指令类型注册不同的消息处理程序
     */
    enum class InstructionType{
        Repeat, // 复读
        CommercialAccount, // 营销号
        UselessParagraph, // 废话
        Trash, // 垃圾
        Sick, // 发病
        KFCCrazyThursday, // 疯狂星期四V我50
        TouhouProject, // 东方Project
        SignOn, // 签到
        Fishing, // 钓鱼
        Working, // 打工
        FiveThousandMega, // 5000兆表情包生成
        Menu, // 菜单
        About, // 关于
        NotImplemented, // 未实现
        OtherMessage // 普通聊天消息
    };

    /**
     * @brief 结构体 SingleInstruction
     * 代表单条指令的结构
     */
    struct SingleInstruction {
        const MiraiCP::GroupMessageEvent& event;
        InstructionType type;
        std::vector<std::string> arguments;
    };

    /**
     * @brief 消息处理过程的定义，签名为const SingleInstruction& -> void
     */
    using InstructionProcedure = std::function<void(const SingleInstruction&)>;

    // 全局数据结构定义

    /**
     * @brief 消息队列
     */
    extern std::queue<MiraiCP::GroupMessageEvent> messageQueue;

    /**
     * @brief 控制消息队列更改的互斥锁
     */
    extern std::mutex messageQueueMutex;

    /**
     * @brief 控制消息队列更改的条件变量
     */
    extern std::condition_variable msgQueueCondVariable;

    /**
     * @brief 消息处理函数映射表
     */
    extern std::unordered_map<InstructionType, InstructionProcedure,
                std::hash<InstructionType>> instructionProcedureMap;

    // 接口

    /**
     * @brief produceMessage 适用于producer的接口，往消息队列末尾放入一条消息
     * @param event 待处理的消息
     */
    void produceMessage(const MiraiCP::GroupMessageEvent& event);

    /**
     * @brief translateMessage 将文字消息转换为SingleInstruction结构
     * @param event 待处理的群消息
     * @return 代表转换后的单条指令
     */
    SingleInstruction translateMessage(const MiraiCP::GroupMessageEvent& event);

    /**
     * @brief 查询text字符串对应的指令信息
     * @param text 待查询的字符串
     * @return 若text与某一条指令匹配，返回对应的指令；
     * 若text不与任意一条指令匹配，返回InstructionType::NotImplemented
     */
    InstructionType queryInstructionTypeFromText(const std::string& text);

    /**
     * @brief 将单条指令送往对应的指令处理程序
     * @param instruction 待处理的消息
     */
    void dispatchInstruction(const SingleInstruction& instruction);

    /**
     * @brief 对给定的指令类型，注册对应的指令处理程序
     * @param instructionType 指令类型
     * @param instructionProcedure 指令处理程序
     */
    void registerInstructionProcedure(InstructionType instructionType,
                                      InstructionProcedure instructionProcedure);

    /**
     * @brief 对未注册指令处理程序的指令类型，默认的处理程序
     * @param instruction 指令
     */
    void defaultInstructionProcedure(const SingleInstruction& instruction);

    /**
     * @brief 消费者线程函数，从消息队列队头取出消息并处理
     */
    [[noreturn]]
    void consumerThreadFunction();

    /**
     * @brief 根据delim，将str切割并存储在返回值的vector中
     * @param str 将被分割的字符串
     * @param delimiter 分隔符
     * @return 切割完成后的字符串组成的vector
     */
    std::vector<std::string>
    split(const std::string& str, char delim);

}

// 打开命名空间std，并对InstructionType定义std::hash特化
namespace std{
    template<>
    struct hash<MessageDispatcher::InstructionType>{
        size_t operator() (const MessageDispatcher::InstructionType& instruction) const {
            return std::hash<int>()(static_cast<int>(instruction));
        }
    };
}

#endif //MIRAICPPLUGIN_MESSAGE_DISPATCHER_H
