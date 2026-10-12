## 一、执行走法 
>int makeMove(int fromX, int fromY, int toX, int toY)

1.修改broad的二维数组，记录棋子在后端的移动情况
2.棋子移动后转换player

## 二、棋谱记录
>int recordMove(int toX, int toY, ChessPiece piece)

简化：对小兵吃子要出现from的坐标进行了简化
### （一）转化为标准输出格式
1.棋子类型转换（实际名称-->标准记谱记号）
2.棋子横坐标转换（二维坐标中的数值-->英文字母）
3.棋子轮数
>4.将标准输出格式以char形式临时储存（便于索取）

### （二）棋谱存储
1.文件创建与打开
（1）第一次round，创建文件
（2）round！=1，以读写形式打卡文件，不允许更改
    注意：文件指针移动到文件末尾，便于添加新的记谱信息
2.写入信息
（1）判断黑方白方
（2）如果白方（每一场对弈的第一个下棋的人），输入round
3.文件保存关闭

### （三）棋谱输出
1.获取并拼接三个变量在record char形式数组中
2.调用draw_record函数，传入record(本次记谱内容) 

>int draw_record(char* record_txt, int currentplayer)

图形窗口输出文字,设置透明文字背景、文字颜色、大小、位置

