# 姓名-学校-开局一课客户端大作业说明

## 一、项目基本信息

项目名称：UE5GISScene  
项目类型：基于 Unreal Engine 5.7 的第一人称射击联机 Demo  
工程路径：E:\UnrealProjects\UE5GISScene  
GitHub 地址：https://github.com/wei3511162263-jpg/UE5GISScene.git  
默认地图：/Game/_Main/map/NewMap  
主要角色蓝图：/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter  

本项目基于 UE5 官方 First Person 模板继续开发，在原有第一人称移动、视角控制和基础射击框架上，扩展了第三人称可见角色、武器动作、射击命中反馈、弹药与换弹、玩家生命值、死亡重生以及多人网络同步功能，形成一个可以在 PIE 双窗口中进行基础对战测试的 FPS Demo。

## 二、作业要求对应完成情况

### 1. 角色移动和攻击敌人功能

项目保留并扩展了 First Person 模板中的基础移动控制，玩家可以使用键盘控制前后左右移动，使用鼠标控制视角方向，并通过鼠标射击。工程中使用 Enhanced Input 输入系统，主要输入资产包括 IA_shoot、IA_R 和 IA_Sprint。

射击功能由 BP_FirstPersonCharacter 中的射击逻辑实现。玩家按下射击输入后，会通过服务器事件执行实际开火判断。射击使用 Line Trace 检测命中对象，并根据命中结果生成弹孔、命中特效和伤害。项目中还加入了弹药数量限制，子弹耗尽后不能继续射击，需要按 R 进行换弹。

在攻击目标方面，项目中包含 target 蓝图和玩家受击逻辑。射线命中目标后可以触发目标反馈；命中其他玩家时，会通过 Apply Damage 触发玩家角色的 Event AnyDamage 事件，从而扣除 Health。

### 2. 得分和胜利机制

本 Demo 当前实现了基础对战闭环，主要胜负逻辑以“击败对手”为核心：玩家被射击命中后会扣除 Health，当 Health 小于等于 0 时触发死亡逻辑。死亡后角色会隐藏并关闭碰撞，经过短暂延迟后恢复生命值并在出生点复活。

由于本次 Demo 的重点是完成可联机的 FPS 基础玩法，得分机制采用可扩展设计：击杀事件已经可以通过 Health <= 0 的死亡分支作为得分触发点。后续可以在该分支中增加 Score 变量、击杀者记录、分数 HUD 显示以及达到指定分数后结束比赛的胜利判断。

当前版本已经具备实现完整胜利规则所需的关键事件入口：射击命中、伤害结算、死亡判断和重生流程。

### 3. 多人网络对战

项目实现了基础多人网络对战流程。射击、换弹、开火特效和第三人称动作均按网络职责进行了拆分：

- 客户端负责接收玩家输入，例如射击、换弹和奔跑。
- 服务器负责关键 Gameplay 判断，例如是否有子弹、射线命中、扣除弹药、造成伤害、补充弹药。
- Multicast 事件负责广播表现效果，例如第三人称开火动作、换弹动作和命中特效。
- 拥有者客户端负责更新自己的 HUD，例如当前弹药数量显示。

在 PIE 双窗口测试中，两个玩家可以看到彼此角色和动作。服务器与客户端均可以射击，命中墙体时可以同步显示弹孔或命中特效，命中玩家时可以触发生命值扣除和死亡重生。

## 三、主要功能实现过程

### 1. 第三人称可见角色搭建

原 First Person 模板只适合自己看到手臂，不适合其他玩家观察角色。为了实现多人对战，本项目在 BP_FirstPersonCharacter 中补充了第三人称 Bodymesh，并为其配置动画蓝图 abp_player_tp。

第三人称动画使用 Rifle 动作资源，包括 Idle、Walk、Jog、Jump、Fire、Reload 等状态。移动部分使用 Blend Space 处理 Idle、Walk、Jog 三种状态，并结合速度变量控制动画过渡。奔跑时通过 Character Movement 的 Max Walk Speed 切换移动速度，使角色在其他玩家视角中也能体现移动状态变化。

### 2. 瞄准偏移和上下看同步

为了让第三人称角色在其他玩家视角中能表现抬头和低头，本项目制作并接入了 Aim Offset。动画蓝图中读取角色控制旋转与角色自身旋转的差值，将 Pitch 传入 Aim Offset，使第三人称上半身可以随玩家鼠标上下移动而改变姿态。

调试过程中发现客户端视角的 Pitch 不能简单依赖服务器当前旋转，因此最终通过适合网络环境的变量传递方式，使客户端和其他观察者都能看到正确的上下瞄准表现。

### 3. 联机射击与命中特效

射击逻辑从本地输入开始，但实际命中判断放到服务器执行。客户端按下射击后调用 Server_Fire，服务器检查弹药并执行 Line Trace。为解决客户端上下瞄准时服务器射线方向不正确的问题，最终让客户端把计算好的 Trace Start 和 Trace End 传给服务器，由服务器根据该方向执行命中检测。

命中后，服务器调用 Multicast_ImpactFX，在所有客户端生成对应的命中表现。弹孔或命中特效的位置来自 Hit Result 的 Impact Point，朝向根据 Impact Normal 计算，使弹孔可以贴合墙面或被命中的表面。

### 4. 弹药和换弹逻辑

项目加入了 Currentammo 和 Max Ammo 变量控制弹药。射击时服务器负责扣除 Currentammo，Currentammo 为 0 时不能继续射击。换弹时，客户端按 R 后播放本地第一人称换弹动画，并调用服务器换弹事件 sercer_reload。

sercer_reload 在服务器上执行真正的补弹逻辑，将 Currentammo 设置为 Max Ammo。之后通过 Client_ReloadFinished 通知拥有该角色的客户端更新 HUD。第三人称换弹动作则通过 multicast_reloadrx 广播给所有玩家，使其他客户端也能看到该玩家正在换弹。

这个设计解决了“客户端 HUD 已经显示补弹，但服务器仍认为没有子弹”的问题，保证了联机环境下弹药逻辑以服务器为准。

### 5. 生命值、受击、死亡和重生

项目为 BP_FirstPersonCharacter 增加 Health 变量，默认生命值为 100。服务器射线命中玩家后使用 Apply Damage 对被命中的 Actor 造成伤害。玩家角色自身通过 Event AnyDamage 接收伤害，并执行 Health = Health - Damage。

当 Health 小于等于 0 时，触发死亡流程：角色隐藏，Actor 碰撞关闭，等待 3 秒后恢复 Health，并重新显示角色、开启碰撞，最后通过 PlayerStart 出生点进行复活。地图中可以放置多个 PlayerStart，复活时可从 PlayerStart 数组中随机选择一个位置，从而实现随机出生。

### 6. 网络职责拆分

为了让多人对战行为稳定，本项目将逻辑拆成三类事件：

- Run on Server：处理必须由服务器决定的内容，例如射击、弹药扣除、补弹、伤害结算。
- Multicast：处理所有玩家都应该看到的表现，例如第三人称开火、换弹和命中特效。
- Run on Owning Client：处理只有本地玩家需要看到的内容，例如 HUD 弹药文本更新。

通过这种拆分，避免了客户端本地状态和服务器权威状态不一致的问题。

## 四、关键资产和蓝图

- BP_FirstPersonCharacter：玩家角色核心蓝图，包含输入、射击、换弹、生命值、死亡重生和网络事件。
- BP_FirstPersonPlayerController：玩家控制器。
- BP_FirstPersonGameMode：默认游戏模式。
- abp_player_tp：第三人称角色动画蓝图。
- bs_rifle_walk / bs_rifle_jog：第三人称移动 Blend Space。
- AO_Rifle / ao_tp_rifle：第三人称瞄准偏移动画资源。
- target：可被射击触发反馈的目标蓝图。
- NewMap：项目默认测试地图。
- IA_shoot、IA_R、IA_Sprint：射击、换弹、奔跑输入动作。

## 五、测试方式

1. 打开 UE5GISScene.uproject。
2. 确认默认地图为 /Game/_Main/map/NewMap。
3. 在编辑器中设置 PIE 玩家数量为 2。
4. 启动两个玩家窗口。
5. 分别测试移动、奔跑、射击、换弹、命中特效、玩家受击、死亡和重生。
6. 测试客户端与服务器互相射击，确认双方都能看到弹孔、第三人称动作和死亡重生表现。

## 六、项目总结

UE5GISScene 从 UE5 First Person 模板出发，完成了一个基础 FPS 联机 Demo。项目重点解决了第一人称模板缺少第三人称表现、客户端与服务器弹药状态不一致、客户端射击上下方向不同步、多人视角下动作不可见、玩家受击和死亡重生等问题。

当前版本已经具备基本对战流程：玩家可以移动、奔跑、射击、换弹、攻击其他玩家，并在生命值归零后死亡和复活。后续可以继续扩展计分板、胜利 UI、房间菜单、更多武器、AI 敌人、音效、死亡动画和正式打包发布流程。

GitHub 项目地址：https://github.com/wei3511162263-jpg/UE5GISScene.git
