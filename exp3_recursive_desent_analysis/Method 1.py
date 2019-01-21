# coding=utf-8
from prettytable import PrettyTable
import copy

file = open("test/input1.txt")
inputline = file.readline()
# notendchar保存所有的非终结符
notendcharlist = []
for i in range(len(inputline)-1):
    if inputline[i] !=' ':
        notendcharlist.append(inputline[i])
print("该文法的所有非终结符有:")
print(notendcharlist)
# endcharlist保存所有的终结符
inputline = file.readline()
endcharlist = []
for i in range(len(inputline)-1):
    if inputline[i] !=' ':
        endcharlist.append(inputline[i])
print("该文法的所有终结符有:")
print(endcharlist)
inputline = file.readline()
# inferdict 保存所有的产生式
inferdict = {}
# 初始化inferdict
for char in notendcharlist:
    inferdict[char] = []
# 读取文法,填入inferdict
print("该文法的所有产生式如下：")
while inputline:
    rs = inputline.replace('\n', '')
    left = rs[0:1]
    right = rs[2:]
    inferdict[left].append(right)
    inputline = file.readline()
file.close()
# 打印文法
for i in range(len(notendcharlist)):
    print("%s -> " % notendcharlist[i], end='')
    print(" | ".join(inferdict[notendcharlist[i]]))
print(inferdict)


# 维护一个可用大写字母表，用作新的非终结符
letters = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"
char_can_use = letters.split()
for item in char_can_use:
    if item in notendcharlist:
        char_can_use.remove(item)


# 判断文法的某个非终结符是否存在直接左递归
def if_has_left_recursion(_left, _inferdict):
    # _left是一个非终结符号
    # _inferdict是该文法的推导字典
    for i in _inferdict[_left]:
        if i[0] == _left:
            return True
    return False

# 消除一个非终结符的所有直接左递归,并返回新的_inferdict
def avoid_straight_left_recursion(_left, _inferdict):
    # 声明以下两个list为全局变量方便相关函数改写
    global char_can_use
    global notendcharlist
    # _left是一个非终结符号
    # _inferdict是该文法的推导字典
    # _group1保存所有有直接左递归的产生式
    _group1 = []
    # _group2保存其他产生式
    _group2 = []
    for i in _inferdict[_left]:
        if i[0] == _left:
            _group1.append(i)
        else:
            _group2.append(i)
    # 从char_can_use中弹出一个作为新非终结符
    _new_not_end_char = char_can_use.pop()
    notendcharlist.append(_new_not_end_char)
    # 更改产生式
    _inferdict[_left]=[]
    for i in _group2:
        i = i + _new_not_end_char
        _inferdict[_left].append(i)
    _inferdict[_new_not_end_char] = []
    # 产生新推导式
    for i in _group1:
        i = i.replace(_left,"")
        i = i + _new_not_end_char
        _inferdict[_new_not_end_char].append(i)
    # 在新终结符号的推导式中加入一条ε
    if 'ε' not in _inferdict[_new_not_end_char]:
        _inferdict[_new_not_end_char].append("ε")
    return _inferdict

# 消除一个文法的所有左递归，包括间接左递归,返回处理后的 _inferdict
def avoid_all_left_recursion(_inferdict):
    _inferdict_copy = copy.deepcopy(_inferdict)
    # 按照notendcharlist中的顺序排序
    for i in range(len(notendcharlist)):
        for j in range(0,i):
            # 改写文法
            for k in _inferdict_copy[notendcharlist[i]]:
                if k[0] == notendcharlist[j]:
                    _temp_k = k
                    # 先去掉要改写的规则
                    _inferdict_copy[notendcharlist[i]].remove(k)
                    # 增加新的规则
                    for q in _inferdict_copy[notendcharlist[j]]:
                        _new_right = _temp_k.replace(notendcharlist[j],q)
                        _inferdict_copy[notendcharlist[i]].append(_new_right)
        if if_has_left_recursion(notendcharlist[i], _inferdict_copy) == True:
            _inferdict_copy = avoid_straight_left_recursion(notendcharlist[i], _inferdict_copy)
    return _inferdict_copy

print("消除所有左递归后的文法如下：")
inferdict = avoid_all_left_recursion(inferdict)
for i in range(len(notendcharlist)):
    print("%s -> " % notendcharlist[i], end='')
    print(" | ".join(inferdict[notendcharlist[i]]))

# 求两个字符串的最长公共前缀
def return_longest_prefix(str1, str2):
    _min_lenth = min(len(str1), len(str2))
    _longest_prefix = ""
    for i in range(_min_lenth):
        if str1[i] == str2[i]:
            _longest_prefix = _longest_prefix + str1[i]
        else:
            break
    if len(_longest_prefix) > 0:
        return _longest_prefix
    else:
        return None

# 判断对于某非终结符号的所有产生式，是否有左公共因子
def if_has_left_divisor(_left, _inferdict):
    _perfix_matrix = []
    # 初始化矩阵
    _count = len(_inferdict[_left])
    for i in range(_count):
        _temp = []
        for j in range(_count):
            _temp.append([])
        _perfix_matrix.append(_temp)
    # _all_prefix保存找出的所有可提取前缀，用于之后比较长度
    _all_prefix = []
    # 在矩阵中填入两两之间的最长公共前缀
    for i in range(_count):
        for j in range(i + 1, _count):
            _temp_perfix = return_longest_prefix(_inferdict[_left][i], _inferdict[_left][j])
            if _temp_perfix != None:
                _perfix_matrix[i][j] = _temp_perfix
                if _temp_perfix not in _all_prefix:
                    _all_prefix.append(_temp_perfix)
    # 如果一个可提取前缀也没有
    if len(_all_prefix) == 0:
        return False
    else:
        return True

# 提取左公共因子
def exreact_left_divisor(_left, _inferdict):
    global char_can_use
    global notendcharlist
    # _left是一个非终结符号
    # _inferdict是该文法的推导字典

    # 构造一个"推导式右部公共前缀矩阵"
    _perfix_matrix = []
    # 初始化矩阵
    _count = len(_inferdict[_left])
    for i in range(_count):
        _temp= []
        for j in range(_count):
            _temp.append([])
        _perfix_matrix.append(_temp)
    # _all_prefix保存找出的所有可提取前缀，用于之后比较长度
    _all_prefix = []
    # 在矩阵中填入两两之间的最长公共前缀
    for i in range(_count):
        for j in range(i+1, _count):
            _temp_perfix = return_longest_prefix(_inferdict[_left][i], _inferdict[_left][j])
            if _temp_perfix!= None:
                _perfix_matrix[i][j]=_temp_perfix
                if _temp_perfix not in _all_prefix:
                    _all_prefix.append(_temp_perfix)
    # 找出本次可以提取的最长前缀 _longest_prefix
    _longest_prefix = _all_prefix[0]
    for k in _all_prefix:
        if len(k) > len(_longest_prefix):
            _longest_prefix = k
    # 改写文法
    # ①找出含有该前缀的所有产生式右部,放入_modify_list
    _modify_list = []
    for i in range(_count):
        for j in range(i+1, _count):
            if _perfix_matrix[i][j] == _longest_prefix:
                if _inferdict[_left][i] not in _modify_list:
                    _modify_list.append(_inferdict[_left][i])
                if _inferdict[_left][j] not in _modify_list:
                    _modify_list.append(_inferdict[_left][j])
    # ②在原推导字典中删去这些表达式
    for q in _modify_list:
        _inferdict[_left].remove(q)
    # ③加入提取公因子后的产生式
    _new_not_end_char = char_can_use.pop()
    notendcharlist.append(_new_not_end_char)
    _new_right = _longest_prefix + _new_not_end_char
    _inferdict[_left].append(_new_right)
    # ④增加新终结符A'及其产生式(注意ε)
    _inferdict[_new_not_end_char] = []
    for p in _modify_list:
         _right = p.replace(_longest_prefix,"")
         if _right =="":
             _inferdict[_new_not_end_char].append("ε")
         else:
             _inferdict[_new_not_end_char].append(_right)
    return _inferdict


for ch in notendcharlist:
    if if_has_left_divisor(ch,inferdict) == True:
        inferdict = exreact_left_divisor(ch,inferdict)
print("提取左公因子后的文法如下：")
for i in range(len(notendcharlist)):
    print("%s -> " % notendcharlist[i], end='')
    print(" | ".join(inferdict[notendcharlist[i]]))

# 分析函数
def fun(_state, _inputstr, _inferdict):
    if len(_state)==0 and _inputstr == "$":
        return 'acc'
    _state_copy = copy.deepcopy(_state)
    _inputstr_copy = copy.deepcopy(_inputstr)
    _now_state = _state[0]
    if(_now_state) in endcharlist:
        return 'error'
    else:
        _count = len(_inferdict[_now_state])
        if _count == 1:
            _state_copy = _state_copy.replace(_now_state,_inferdict[_now_state][0], 1)
        else:
            flag = 0 # flag标志是否找到一条可替换的产生式
            for k in _inferdict[_now_state]:
                if k[0] == _inputstr_copy[0]:
                    flag = 1
                    _state_copy = _state_copy.replace(_now_state, k, 1)
                    break
            # 如果找不到
            if flag == 0:
                if 'ε' in _inferdict[_now_state]:
                    _state_copy = _state_copy.replace(_now_state, "",1)
                else:
                    return 'error'
    _rt = return_longest_prefix(_state_copy, _inputstr_copy)
    if _rt != None:
        _state_copy = _state_copy.replace(_rt, "", 1)
        _inputstr_copy = _inputstr_copy.replace(_rt, "", 1)
    return (_state_copy, _inputstr_copy)

# judge_string = input("请输入要分析的字符串：")

s0 = "(i)" #合法
s1 = "(i*i)+i+i" #合法
s2 = "i+i*((i)+(i))+(i+i)+i" #合法
s3 = "i*i*i*i" #合法
s4 = "*(i+i)*(i)*" #不合法
s5 = "+((i)*(i))+(i+i*i)" #不合法
s8 = "i+i+" # 不合法
s9 = "i++" # 不合法


judge_string = s8
judge_string = judge_string + "$"
state = ""
# 隐性要求：在输入文法时必须将开始符号放在第一个
state = state + notendcharlist[0]
print("要判断的字符串是：%s" % judge_string)
print("该字符串的推导过程如下：")
print((state,judge_string))
break_flag = 0
while True:
    rusult = fun(state, judge_string, inferdict)
    if rusult == "acc":
        print("合法！")
        break_flag = 1
    elif rusult == "error":
        print("不合法！")
        break_flag = 1
    else:
        (state, judge_string) = rusult
        print((state, judge_string))
    if break_flag == 1:
        break
