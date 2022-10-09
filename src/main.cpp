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

// MiraiCP依赖文件(只需要引入这一个)
#include <MiraiCP.hpp>
#include "MessageDispatcher.h"
#include "InstructionProcessor.h"
#include "PersonCollection.h"

using namespace MiraiCP;

const PluginConfig CPPPlugin::config{
        "0",          // 插件id
        "Lucas' Bot",        // 插件名称
        "alpha-0.1.1",            // 插件版本
        "Lucas"       // 插件作者
};

// 插件实例
class Main : public CPPPlugin {
public:
  // 配置插件信息
  Main() : CPPPlugin() {}
  ~Main() override = default;

  // 入口函数
  void onEnable() override {
      Logger::logger.info("Hello, world!");
      // 启动消费者线程
      std::thread consumerThread{MessageDispatcher::consumerThreadFunction};
      consumerThread.detach();
      // 载入本地文件
      PersonCollection::registerLocalFile("personMap.json");
      PersonCollection::loadPersonMap();
      // 注册消息处理函数
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::Repeat,
                                                      InstructionProcessor::repeat);
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::CommercialAccount,
                                                          InstructionProcessor::commercialAccount);
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::UselessParagraph,
                                                          InstructionProcessor::uselessParagraph);
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::Trash,
                                                          InstructionProcessor::trash);
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::Sick,
                                                            InstructionProcessor::sick);
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::KFCCrazyThursday,
                                                              InstructionProcessor::kfcCrazyThursday);
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::TouhouProject,
                                                              InstructionProcessor::touhouProject);
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::SignOn,
                                                              InstructionProcessor::signOn);
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::Menu,
                                                              InstructionProcessor::menu);
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::About,
                                                              InstructionProcessor::about);
      MessageDispatcher::registerInstructionProcedure(MessageDispatcher::InstructionType::OtherMessage,
                                                      InstructionProcessor::otherMessage);
      // 添加消息监听事件
      Event::registerEvent<GroupMessageEvent>([](GroupMessageEvent event) {
          MessageDispatcher::produceMessage(event);
      });
  }

  // 退出函数
  void onDisable() override {
        /*插件结束前执行*/
      // 保存本地文件
      PersonCollection::savePersonMap();
  }
};

// 绑定当前插件实例
void MiraiCP::enrollPlugin() {
  MiraiCP::enrollPlugin(new Main);
}
