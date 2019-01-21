# coding=utf-8
# compiler-exp2
import copy
from prettytable import PrettyTable

# 读取文件
file = open("test/NFA1.txt")
statecnt_s = file.readline()
endcnt_s = file.readline()
statecnt1 = int(statecnt_s)
endcnt = int(endcnt_s)
print("该自动机的初始状态数为: %d" % statecnt1)
endstates = []
for i in range(endcnt):
    temp_s = file.readline()
    temp = int(temp_s)
    endstates.append(temp)
print("该自动机的终结状态集为: ", end='')
for i in endstates:
    print(i, end=' ')
print('\r')
charcnt_s = file.readline()
charcnt = int(charcnt_s)
charlist = ["*"]
for i in range(charcnt):
    char_s = file.readline()
    charlist.append(char_s[0])
rulecnt_s = file.readline()
rulecnt = int(rulecnt_s)
print("该自动机的转移边条数有: %d" % rulecnt)

# 构造statedict1,即原始的状态字典
statedict1 = {}
for i in range(0, statecnt1):
    tempstate = []
    if (i in endstates):
        tempstate.append(True)
    else:
        tempstate.append(False)
    states = set()
    states.add(i)
    tempstate.append(states)
    statedict1[i] = tempstate
print("该NFA的初始状态共有:")
print(statedict1)

# 初始化changetbl1,即最初的状态转换表(字典)
changetbl1 = {}
for i in range(0, statecnt1):
    changedict = {}
    for j in charlist:
        # 初始化时都插入一个-1
        changedict[j] = set([-1]);
    changetbl1[i] = changedict

# 将输入的转移边填入初始转换表changetbl1
for i in range(rulecnt):
    rule = file.readline()
    _index = rule.index('-')
    left = int(rule[0:_index])
    op = rule[_index + 1]
    right = int(rule[_index + 3:])
    changetbl1[left][op].add(right)
print("该自动机的初始转移表为:")
# print(changetbl1)
fisrttable = PrettyTable(["当然状态", "输入*", "输入a", "输入b"])
for i in range(statecnt1):
    fisrttable.add_row([i, changetbl1[i]['*'], changetbl1[i]['a'], changetbl1[i]['b']])
print(fisrttable)


# 判断某个状态集合中是否包括终结状态
def ifendset(_set):
    for _item in _set:
        if _item in endstates:
            return True
    return False


# 找出返回集合中的新元素
def findnew(_eset, _return):
    newlist = []
    for elem in _return:
        if elem not in _eset:
            newlist.append(elem)
    return newlist


# 判断一个状态是否可以转移
def ifnull(_judgeset):
    if len(_judgeset) == 1 and -1 in _judgeset:
        return False
    else:
        return True


# 查找某一状态集合是否是从未出现的新集合
def ifnewstate(_inpout, _statelist):
    for _s in _statelist:
        if (ifequ(_s, _inpout) == True):
            return False
    return True


# 求一个状态的e-闭包
e_set = set()  # 存放每次e-闭包计算的结果,用完之后需要及时清空重置
e_set.add(-1)


def eclosure(_stateid, _changetbl):
    global e_set
    if ifnull(_changetbl[_stateid]['*']) == False:
        e_set.add(-1)
    else:
        newelem = findnew(e_set, _changetbl[_stateid]['*'])
        e_set = e_set.union(_changetbl[_stateid]['*'])
        for q in newelem:
            eclosure(q, _changetbl)


# 重置e_set函数
def cleaneset():
    global e_set
    e_set.clear()
    e_set.add(-1)


# 判断该状态集是否是新状态集
def search_new_stateset(_set):
    _set_a_noe = set([-1])
    _set_b_noe = set([-1])
    for _elem in _set:
        if _elem == -1: continue
        _set_a_noe = _set_a_noe.union(changetbl1[_elem]['a'])
        _set_b_noe = _set_b_noe.union(changetbl1[_elem]["b"])
    _set_a = copy.deepcopy(_set_a_noe)
    _set_b = copy.deepcopy(_set_b_noe)
    for _ele1 in _set_a_noe:
        if _ele1 == -1:
            continue
        else:
            eclosure(_ele1, changetbl1)
            _set_a = _set_a | e_set
            cleaneset()
    for _ele2 in _set_b_noe:
        if _ele2 == -1:
            continue
        else:
            eclosure(_ele2, changetbl1)
            _set_b = _set_b | e_set
            cleaneset()
    if (_set_a not in statelist2) and (ifequ(_set_a, set([-1])) == False):
        statelist2.append(_set_a)
    if (_set_b not in statelist2) and (ifequ(_set_b, set([-1])) == False):
        statelist2.append(_set_b)


# 返回某个状态集接受字符a,b后转移到的状态集
def return_next_state(_set):
    _set_a_noe = set([-1])
    _set_b_noe = set([-1])
    for _elem in _set:
        if _elem == -1: continue
        _set_a_noe = _set_a_noe.union(changetbl1[_elem]['a'])
        _set_b_noe = _set_b_noe.union(changetbl1[_elem]["b"])
    _set_a = copy.deepcopy(_set_a_noe)
    _set_b = copy.deepcopy(_set_b_noe)
    for _ele1 in _set_a_noe:
        if _ele1 == -1:
            continue
        else:
            eclosure(_ele1, changetbl1)
            _set_a = _set_a | e_set
            cleaneset()
    for _ele2 in _set_b_noe:
        if _ele2 == -1:
            continue
        else:
            eclosure(_ele2, changetbl1)
            _set_b = _set_b | e_set
            cleaneset()
    return (_set_a, _set_b)


# 判断两个状态集是否相等
def ifequ(_judgeset1, _judgeset2):
    if len(_judgeset1) != len(_judgeset2):
        return False
    else:
        for elem in _judgeset1:
            if elem not in _judgeset2:
                return False
        return True


# 找出某一状态集合在NFA中对应的状态ID
def find_state_id(_dict, _set):
    for i in range(len(_dict)):
        if ifequ(_set, _dict[i][1]) == True:
            return i


# 寻找某一状态集合在一个DFA中的状态ID
def DFA_return_index(_tbl1set):
    _statelist = []
    for i in range(statecnt1):
        _state = set([-1])
        _state.add(i)
        _statelist.append(_state)
    for k in range(statecnt1):
        if (ifequ(_tbl1set, _statelist[k]) == True):
            return k


# 判断是NFA还是DFA
def judge(_statecnt, _charlist, _changetbl):
    for i in range(0, _statecnt):
        for j in _charlist:
            if len(_changetbl[i][j]) >= 3:
                print("该自动机是一个NFA.")
                return True
    print("该自动机是一个DFA.")
    return False


# 如果该自动机是NFA
if judge(statecnt1, charlist, changetbl1) == True:
    # 子集构造算法,NFA转换DFA
    # 找出化简后DFA的初始状态
    eclosure(0, changetbl1)
    statelist2 = []
    startset2 = copy.deepcopy(e_set)
    cleaneset()
    startset2.add(0)
    statelist2.append(startset2)
    # 模拟do...while语句实现DFA所有状态的查找
    o = 0
    while True:
        search_new_stateset(statelist2[o])
        o = o + 1
        if o == len(statelist2):
            break
    # 将所有新状态填入statedict2
    statedict2 = {}
    for i in range(len(statelist2)):
        _temps = []
        _temps.append(ifendset(statelist2[i]))
        _temps.append(statelist2[i])
        statedict2[i] = _temps
    print("该NFA转换为DFA后的所有状态如下:")
    print(statedict2)
    # 构造DFA的状态转换表(字典)changetbl2
    changetbl2 = {}
    for i in range(len(statedict2)):
        (_a, _b) = return_next_state(statedict2[i][1])
        index_a = find_state_id(statedict2, _a)
        index_b = find_state_id(statedict2, _b)
        changedict2 = {}
        changedict2['a'] = index_a
        changedict2['b'] = index_b
        changetbl2[i] = changedict2
    print("该NFA转换为DFA后的状态转移表如下:")
    # 打印转换表
    print(changetbl2)
    table = PrettyTable(["当然状态", "输入a", "输入b"])
    for i in range(len(statedict2)):
        table.add_row([i, changetbl2[i]['a'], changetbl2[i]['b']])
    print(table)

# 如果该自动机是DFA
else:
    statedict2 = copy.deepcopy(statedict1)
    changetbl2 = {}
    for i in range(statecnt1):
        changedict2 = {}
        for j in ["a", "b"]:
            index = DFA_return_index(changetbl1[i][j])
            changedict2[j] = index
        changetbl2[i] = changedict2
    print("该DFA的状态转移表如下:")
    table = PrettyTable(["当然状态", "输入a", "输入b"])
    for i in range(statecnt1):
        table.add_row([i, changetbl2[i]['a'], changetbl2[i]['b']])
    print(table)

statecnt2 = len(statedict2)


# 求同法判别等价对
# 等价规则1:一个是终结状态,一个是非终结状态,这两个一定不等价
# 判断一个状态是否是终结状态
def if_end_state(_stateid, _statedict):
    return _statedict[_stateid][0]


# 返回一个状态转移边的条数
def return_rules(_stateid, _changetbl):
    _charlist = []
    for i in ["a", "b"]:
        if (_changetbl[_stateid][i] != None):
            _charlist += i
    return _charlist


# 等价规则2:如果两个状态转移边数量不同,这两个一定不等价
def if_same_in_rules(_stateidA, _stateidB, _changetbl):
    _A_charlist = return_rules(_stateidA, _changetbl)
    _B_charlist = return_rules(_stateidB, _changetbl)
    return _A_charlist == _B_charlist


# 等价规则3:如果两个状态,在输入相同的字符时,一个转移到了非终结状态,一个转移到了终结状态,这两个一定不等价
def if_change_to_similar_state(_stateidA, _stateidB, _statedict, _changetbl):
    _rules = return_rules(_stateidA, _changetbl)
    for i in _rules:
        if (if_end_state(_changetbl[_stateidA][i], _statedict) != if_end_state(_changetbl[_stateidB][i], _statedict)):
            return False
    return True


def if_dengjia_state(_stateidA, _stateidB, _statedict, _changetbl):
    next_judge = []
    if if_end_state(_stateidA, _statedict) != if_end_state(_stateidB, _statedict):  # 如果不满足规则1
        return False
    else:
        if if_same_in_rules(_stateidA, _stateidB, _changetbl) == False:  # 如果不满足规则2
            return False
        else:
            # 如果不满足规则3
            if if_change_to_similar_state(_stateidA, _stateidB, _statedict, _changetbl) == False:
                return False
            else:
                # 满足1,2,3 并且对于每个输入都转移到相同的状态,肯定等价
                if (_changetbl[_stateidA]["a"] == _changetbl[_stateidB]["a"]) and _changetbl[_stateidA]["b"] == \
                        _changetbl[_stateidB]["b"]:
                    return True
                else:
                    # 需要进一步判断,返回下一步判断需要的"判断字典"
                    _state_tuple = (_stateidA, _stateidB)
                    _next_judge_dict = {}
                    _next_judge_list = []
                    for _char in ["a", "b"]:
                        if _changetbl[_stateidA][_char] != _changetbl[_stateidB][_char]:
                            if _changetbl[_stateidA][_char] < _changetbl[_stateidB][_char]:
                                _next_judge_list.append((_changetbl[_stateidA][_char], _changetbl[_stateidB][_char]))
                            else:
                                _next_judge_list.append((_changetbl[_stateidB][_char], _changetbl[_stateidA][_char]))

                    _next_judge_dict[_state_tuple] = _next_judge_list
                    return (_state_tuple, _next_judge_dict)


sure_dengjia_list = []  # 存放已经确定的等价对
sure_not_dengjia_list = []  # 存放已经确定不等价的状态对
next_judge_tuple_list = []  # 存放还需要下一步验证的状态对
next_judge_dict = {}  # 存放还需要下一步验证的状态对和它的判别依赖对
for i in range(statecnt2):
    for j in range(i + 1, statecnt2):
        if if_dengjia_state(i, j, statedict2, changetbl2) == False:
            sure_not_dengjia_list.append((i, j))
        else:
            if if_dengjia_state(i, j, statedict2, changetbl2) == True:
                sure_dengjia_list.append((i, j))
            else:
                (temp_tuple, temp_dict) = if_dengjia_state(i, j, statedict2, changetbl2)
                next_judge_tuple_list.append(temp_tuple)
                next_judge_dict.update(temp_dict)

print("已经确定的等价对有:")
print(sure_dengjia_list)
print("已经确定的非等价对有:")
print(sure_not_dengjia_list)
print("还需进一步判定的等价对有:")
print(next_judge_dict)

# 循环找出所有一定不等价状态
if_modify_not_dengjia_list = 1  # if_modify_not_dengjia_list == 1时说明上一次循环从next_judge_dict找出了至少一对不等价状态
while (if_modify_not_dengjia_list != 0):
    _if_modify_not_dengjia_list = 0
    for i in next_judge_tuple_list:
        for j in range(len(next_judge_dict[i])):
            if next_judge_dict[i][j] in sure_not_dengjia_list:
                next_judge_tuple_list.remove(i)
                next_judge_dict.pop(i)
                sure_not_dengjia_list.append(i)
                _if_modify_not_dengjia_list = 1
    if_modify_not_dengjia_list = _if_modify_not_dengjia_list

# 剩下所有状态都等价
for i in next_judge_tuple_list:
    next_judge_tuple_list.remove(i)
    next_judge_dict.pop(i)
    sure_dengjia_list.append(i)

sure_dengjia_pairs = []
# 转换sure_dengjia_list里不可遍历的tuple为list
for _tuples in sure_dengjia_list:
    _t_list = list(_tuples)
    sure_dengjia_pairs.append(_t_list)
print("最终确定的等价对有:")
print(sure_dengjia_pairs)

# 合并等价对(在原表上删除等价的行)
changetbl3 = copy.deepcopy(changetbl2)
statedict3 = copy.deepcopy(statedict2)
while len(sure_dengjia_pairs) != 0:
    pair = sure_dengjia_pairs[0]
    sure_dengjia_pairs.remove(pair)
    if pair[0] == pair[1]:
        continue
    else:
        changetbl3.pop(pair[1])
        statedict3.pop(pair[1])
        # 遍历转换表
        for i in changetbl3:
            for j in ["a", "b"]:
                if changetbl3[i][j] == pair[1]:
                    changetbl3[i][j] = pair[0]
        # 遍历后续等价对
        for i in sure_dengjia_pairs:
            for j in range(0, 2):
                if i[j] == pair[1]:
                    i[j] = pair[0]

for i in statedict3:
    if i == 0:
        statedict3[i][0] = "Start"
    elif ifendset(statedict3[i][1]) == True:
        statedict3[i][0] = "End"
    else:
        statedict3[i][0] = "Common"

# 输出最小化后的状态转移表
print("最小化后的所有状态如下:")
print(statedict3)
print("最小化后的状态转移表如下:")
table2 = PrettyTable(["当然状态", "输入a", "输入b"])
for i in changetbl3:
    table2.add_row([i, changetbl3[i]['a'], changetbl3[i]['b']])
print(table2)

# 对输入的字符串进行判断
while 1:
    cur_state = 0
    judge_string = input("请输入要判断的字符串:")
    for i in judge_string:
        if i not in ["a", "b"]:
            print("请输入正确的字符串!")
            break
        else:
            cur_state = changetbl3[cur_state][i]

    if statedict3[cur_state][0] == "End":
        print("Yes!")
    else:
        print("No!")
