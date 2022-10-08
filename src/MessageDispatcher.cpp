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

#include "MessageDispatcher.h"

#include <utility>

namespace MessageDispatcher {
    std::queue<MiraiCP::GroupMessageEvent> messageQueue;

    std::mutex messageQueueMutex;

    std::condition_variable msgQueueCondVariable;

    std::unordered_map<MessageDispatcher::InstructionType,
            MessageDispatcher::InstructionProcedure> instructionProcedureMap;
}

std::vector<std::string> MessageDispatcher::split(const std::string &str, char delim) {
    std::vector<std::string> res;
    std::stringstream ss(str);
    std::string temp;
    while (getline(ss, temp, delim)) {
        res.push_back(temp);
    }
    return res;
}

void MessageDispatcher::produceMessage(const MiraiCP::GroupMessageEvent &event) {
    // 锁定消息队列的互斥锁
    std::unique_lock<std::mutex> lock(messageQueueMutex);
    // 往消息队列末尾放入一条消息
    messageQueue.push(event);
    // 通知消费者线程取出消息
    msgQueueCondVariable.notify_one();
    // std::unique_lock的析构函数会自动释放互斥锁
}

MessageDispatcher::SingleInstruction
MessageDispatcher::translateMessage(const MiraiCP::GroupMessageEvent &event) {
    // 获得消息链
    auto msgChain = event.message;
    // 判断消息类型
    switch(msgChain[0].type()) {
        // 若为纯文本信息
        case (int) MessageDispatcher::MessageType::PlainText: {
            // 获得消息的内容并存储在messageText中
            auto plain = msgChain[0].get<MiraiCP::PlainText>();
            std::string messageText = plain.toJson()["content"].get<std::string>();
            if (messageText[0] == instructionPrefix) {
                // 以空格为界，分开指令和参数
                auto command = messageText.substr(1, messageText.find(' ') - 1);
                auto args = messageText.substr(messageText.find(' ') + 1);
                // 将指令中的字符全部转换为小写（如果有）
                for (auto& c: command) {
                    if (std::isalnum(c))
                        c = static_cast<char>(std::tolower(c));
                }
                // 切割算法存在的一点问题：如果消息中不存在空格，那么参数字符串将会和指令字符串一样
                // 此时需要手动置为空
                if (args == messageText.substr(0, messageText.find(' ') - 1))
                    args.clear();
                // 翻译指令
                auto instructionType = queryInstructionTypeFromText(command);
                // 构建返回值
                return SingleInstruction{
                    .event = event,
                    .type = instructionType,
                    .arguments = split(args, ' ')
                };
            } else {
                return SingleInstruction{
                    .event = event,
                    .type = InstructionType::OtherMessage,
                    .arguments = std::vector<std::string>{}
                };
            }
        }
        default:
            return SingleInstruction{
                    .event = event,
                    .type = InstructionType::OtherMessage,
                    .arguments = std::vector<std::string>{}
            };
    }
}

MessageDispatcher::InstructionType MessageDispatcher::queryInstructionTypeFromText(const std::string &text) {
        if (text == "复读")
            return InstructionType::Repeat;
        else if (text == "菜单")
            return InstructionType::Menu;
        else if (text == "营销号")
            return InstructionType::CommercialAccount;
        else if (text == "发病")
            return InstructionType::Sick;
        else if (text == "废话")
            return InstructionType::UselessParagraph;
        else if (text == "倒垃圾")
            return InstructionType::Trash;
        else if (text == "kfc")
            return InstructionType::KFCCrazyThursday;
        else if (text == "东方" || text == "车万")
            return InstructionType::TouhouProject;
        else if (text == "签到" || text == "打卡")
            return InstructionType::SignOn;
        else if (text == "钓鱼")
            return InstructionType::Fishing;
        else if (text == "打工")
            return InstructionType::Working;
        else if (text == "五千兆")
            return InstructionType::FiveThousandMega;
        else if (text == "关于")
            return InstructionType::About;
        else
            return InstructionType::NotImplemented;
}

void MessageDispatcher::dispatchInstruction(const MessageDispatcher::SingleInstruction &instruction) {
    // 找到instructionProcedureMap中对应的入口点
    // 若未找到，调用defaultInstructionProcedure
    if (instructionProcedureMap.find(instruction.type) == instructionProcedureMap.end()) {
        defaultInstructionProcedure(instruction);
    } else {
        auto entry = instructionProcedureMap.find(instruction.type)->second;
        entry(instruction);
    }
}

void MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType instructionType,
                                                     MessageDispatcher::InstructionProcedure instructionProcedure) {
    // 注册消息处理程序，没啥好说的，直接莽就完了
    // 如果之前注册了，会被替换掉
    instructionProcedureMap[instructionType] = std::move(instructionProcedure);
}

void MessageDispatcher::defaultInstructionProcedure(const MessageDispatcher::SingleInstruction &instruction) {
    // 获得消息来源的组
    auto group = instruction.event.group;
    // 发送消息
    group.sendMessage(MiraiCP::PlainText("未知或未实现的指令"));
}

void MessageDispatcher::consumerThreadFunction() {
    // 消费者线程函数
    // 从消息队列中取出消息并处理
    while (true) {
        // 构造一个std::unique_lock对象，用于锁定互斥量
        std::unique_lock<std::mutex> lock(messageQueueMutex);
        // 如果消息队列为空，那么等待消息队列中有消息
        msgQueueCondVariable.wait(lock, [] { return !messageQueue.empty(); });
        // 取出消息队列中的第一条消息
        auto msg = messageQueue.front();
        // 从消息队列中删除该消息
        messageQueue.pop();
        // 解锁互斥量
        lock.unlock();
        // 翻译消息
        auto instruction = translateMessage(msg);
        // 处理消息
        dispatchInstruction(instruction);
    }
}