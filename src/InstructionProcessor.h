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

#ifndef MIRAICPPLUGIN_INSTRUCTIONPROCESSOR_H
#define MIRAICPPLUGIN_INSTRUCTIONPROCESSOR_H

#include <MiraiCP.hpp>
#include "MessageDispatcher.h"

namespace InstructionProcessor {
    // 重构：
    // 抽取原MessageDispatcher::dispatch函数中的指令处理部分
    // 对每个指令的处理将由指令的处理函数负责
    // 指令的处理函数将由一个map维护
    // 指令处理函数的原型为：void(const MessageDispatcher::SingleInstruction&)

    /**
     * @brief 复读指令处理函数
     * @param instruction 指令
     */
    void repeat(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 营销号指令处理函数
     * @param instruction 指令
     */
    void commercialAccount(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 废话指令处理函数
     * @param instruction 指令
     */
    void uselessParagraph(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 倒垃圾指令处理函数
     * @param instruction 指令
     */
    void trash(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 发病指令处理函数
     * @param instruction 指令
     */
    void sick(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 疯狂星期四V我50指令处理函数
     * @param instruction 指令
     */
    void kfcCrazyThursday(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 东方Project指令处理函数
     * @param instruction 指令
     */
    void touhouProject(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 签到指令处理函数
     * @param instruction 指令
     */
    void signOn(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 钓鱼指令处理函数
     * @param instruction 指令
     */
    void fishing(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 打工指令处理函数
     * @param instruction 指令
     */
    void working(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 菜单指令处理函数
     * @param instruction 指令
     * @author Lucas
     */
    void menu(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 关于指令处理函数
     * @param instruction 指令
     */
    void about(const MessageDispatcher::SingleInstruction& instruction);

    /**
     * @brief 一般消息处理函数
     * @param instruction 指令
     */
    void otherMessage(const MessageDispatcher::SingleInstruction& instruction);
}


#endif //MIRAICPPLUGIN_INSTRUCTIONPROCESSOR_H
