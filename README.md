# TravelSystem

BUPT Course Design of Data Structure

EasyLog：日志文件，对旅客状态变化和键入等信息进行记录

input：根据城市名单city.txt随机生成数据于input.txt

main：根据input.txt为旅客设计旅行线路并输出旅客状态

### 设计要求：
在当前COVID-19疫情环境下，各个城市的风险程度不同，城市之间可通过三种交通工具（汽车、火车和飞机）相连，某旅客于某一时刻向系统提出旅行要求，系统根据该旅客的要求为其设计一条旅行线路并输出；系统能查询当前时刻旅客所处的地点和状态（停留城市/所在交通工具）。
### 旅行策略：
最少风险策略：无时间限制，风险最少即可
限时最少风险策略：在规定的时间内风险最少
### 选做：
为不同交通工具设置不同单位时间风险值，交通工具单位时间风险值分别为：汽车 = 2；火车 = 5；飞机 = 9。旅客乘坐某班次交通工具的风险 = 该交通工具单位时间风险值 * 该班次起点城市的单位风险值 * 乘坐时间。将乘坐交通工具的风险考虑进来，实现前述最少风险策略和限时风险最少策略。

使用堆优化的Dijkstra算法寻找最短路径。将各个城市与交通工具时刻表相结合进行扩点，再将风险值作为边权进行堆优化，同时用最大限定时间进行剪枝。
