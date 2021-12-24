#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <io.h>
// 入学日期
typedef struct date
{
    int year, month, day;
} Date;
// 学生信息
typedef struct student
{
    char stu_id[20];    //学号
    char stu_name[20];  //姓名
    char stu_sex[4];    //性别
    char stu_class[20]; //班级
    Date date;          //入学时间
} Student;
// 学生信息链表
typedef struct node
{
    Student data;
    struct node *next; //下一个节点
} Node;
//撤销任务,增加0,删除1,修改2
typedef struct task
{
    int operate; //已执行的操作
    Node *last;  //记录上一个节点
    Student data;
} Task;
// 记录撤销任务的顺序栈
typedef struct taskdata
{
    Task task[10];
    int top; //栈顶地址
} TaskData;

// 初始化顺序栈
TaskData *Revoke_init();
// 撤销任务入栈
void Input_TaskData(TaskData *S, int operate, Node *last, Student data);
// 撤销任务出栈
void Output_TaskData(TaskData *S, Node *head);
// 初始化学生信息链表
Node *Stu_init(TaskData *S);
// 申请学生信息链表节点内存空间
Node *Apply_StuNode();
// 菜单
void Menu(Node *head, TaskData *S);
// 输入学生信息
void Input_StuNode(Node *head, TaskData *S);
// 输入学号
int Input_Stu_ID(char *str);
// 判断学号是否存在
int Judge_Id(char *str, Node *head);
// 输入性别，男或女
void Input_Stu_Sex(char *str);
// 输入入学日期，年份限定在1900~2021年，
Date Input_Stu_Date();
// 输入字符串，不能为空
void Input_Stu_Other(char *str);
// 输入学号查询学生信息
void Output_StuNodeByID(Node *head);
// 删除学生节点
void Delete_StuNodeByID(Node *head, TaskData *S);
// 修改学生信息
void Updata_StuNodeByID(Node *head, TaskData *S);
// 输出学生节点信息
void Output_StuNode(Node *head);
// 统计班级人数 并查询学生信息
void Statistic_Stu_Class(Node *head);
// 统计男女比例
void Statistic_Stu_Sex(Node *head);
// 统计并查询已毕业的学生
void Statistic_Stu_Graduation(Node *head);
// 保存学生节点信息到文档
void SaveStuNodeToFile(Node *head);
// 计算是否为闰年
int Yearday(int year);
// 返回月份天数
int Month(Date d);
// 判断日期是否相等
int EqualDate(Date d, Date s);
// 获取一年内已度过的天数
int DayOfYear(Date d);
// 返回日期间隔天数
int DateInterval(Date d, Date s);
// ===============================================程序入口====================================
int main()
{
    //撤销功能初始化
    TaskData *S = Revoke_init();
    // 创建学生信息链表头指针，指向第一个节点
    Node *head = Stu_init(S);
    // 循环弹出菜单
    while (1)
    {
        system("CLS");
        Menu(head, S);
        system("PAUSE");
    }
}
// ===========================================================================================
TaskData *Revoke_init()
{
    TaskData *S;
    // 申请空间
    S = (TaskData *)malloc(sizeof(TaskData));
    if (S == NULL)
    {
        printf("申请内存空间失败");
        exit(0);
    }
    // 初始化栈顶位置
    S->top = -1;
    return S;
}
void Input_TaskData(TaskData *S, int operate, Node *last, Student data)
{
    Task a;
    a.data = data;
    a.last = last;
    a.operate = operate;
    // 栈顶坐标+1
    S->top = (S->top + 1) % 10;
    // 数据入栈
    S->task[(S->top) % 10] = a;
}
void Output_TaskData(TaskData *S, Node *head)
{
    Node *a = head;
    if (S->top == -1)
    {
        printf("\n没有可以撤销的内容！\n\n");
        return;
    }
    // 获取需要撤销的操作
    switch (S->task[S->top].operate)
    {
    case 0:
    {
        while (a != NULL)
        {
            if (a == S->task[S->top].last)
            {
                a->next = NULL;
                S->top = S->top - 1;
                SaveStuNodeToFile(head);
                printf("\n撤销成功！\n\n");
                return;
            }
            a = a->next;
        }
    }
        printf("\n撤销失败，无法撤销！\n\n");
        break;
    case 1:
    {
        Node *p = Apply_StuNode();
        p->next = NULL;
        p->data = S->task[S->top].data;
        if (a->next == NULL)
        {
            a->next = p;
            S->top = S->top - 1;
            SaveStuNodeToFile(head);
            printf("\n撤销成功！\n\n");
            return;
        }
        else
        {
            while (a != NULL)
            {
                if (a == S->task[S->top].last)
                {
                    p->next = a->next;
                    a->next = p;
                    S->top = S->top - 1;
                    SaveStuNodeToFile(head);
                    printf("\n撤销成功！\n\n");
                    return;
                }
                a = a->next;
            }
        }
    }
        printf("\n撤销失败，无法撤销！\n\n");
        break;
    case 2:
    {
        while (a != NULL)
        {
            if (a == S->task[S->top].last)
            {
                a->data = S->task[S->top].data;
                S->top = S->top - 1;
                SaveStuNodeToFile(head);
                printf("\n撤销成功！\n\n");
                return;
            }
            a = a->next;
        }
    }
        printf("\n撤销失败，无法撤销！\n\n");
        break;
    default:
        printf("\n撤销失败，无法撤销！\n\n");
        break;
    }
}
Node *Stu_init(TaskData *S)
{
    Node *head;
    head = Apply_StuNode();
    // 初始化头结点
    head->next = NULL;
    if (access("./学生信息管理系统.txt", 0))
    {
        printf("\n欢迎使用学生信息管理系统\n\n");
        printf("本系统包含的功能有：\n");
        printf("添加、修改、删除、查询学生信息\n");
        printf("统计班级人数，统计男女比例、查询毕业人数等功能\n\n");
        printf("\n由于您是第一次使用，需要先录入学生信息\n\n");
        system("PAUSE");
        Input_StuNode(head, S);
        system("CLS");
        printf("\n信息录入成功！这是刚刚录入的学生信息：\n");
        Output_StuNode(head);
        printf("\n\n按任意键进入系统\n");
        system("PAUSE");
    }
    else
    {
        // 读取文件保存的内容
        // 最长1024个字节
        char buf[1024];
        // 存储每行的长度
        int line_len = 0;
        // 打开相对路径下的a.txt文件
        FILE *openFile = fopen("./学生信息管理系统.txt", "r");
        // 循环按行读取
        while (fgets(buf, 1024, openFile))
        {
            // 获取文件长度
            line_len = strlen(buf);
            // 排除换行和回车
            while ('\n' == buf[line_len - 1] || '\r' == buf[line_len - 1])
            {
                buf[line_len - 1] = '\0';
                //去掉换行后，长度减一，然后判断是否内容为空
                line_len--;
            }
            if (0 == line_len)
                continue;
            // 分解字符串，按空格分解
            Node *p = Apply_StuNode();
            p->next = NULL;
            strcpy(p->data.stu_id, strtok(buf, " "));
            strcpy(p->data.stu_name, strtok(NULL, " "));
            strcpy(p->data.stu_sex, strtok(NULL, " "));
            strcpy(p->data.stu_class, strtok(NULL, " "));
            // 获取日期，转换为数字
            p->data.date.year = atoi(strtok(NULL, "/"));
            p->data.date.month = atoi(strtok(NULL, "/"));
            p->data.date.day = atoi(strtok(NULL, "/"));
            // 循环出最后一个节点
            Node *a = head;
            while (a->next != NULL)
                a = a->next;
            // 将这个节点挂在最后一个节点上
            a->next = p;
        }
        fclose(openFile);
    }
    return head;
}
Node *Apply_StuNode()
{
    Node *head;
    head = (Node *)malloc(sizeof(Node));
    if (head == NULL)
    {
        printf("申请内存空间失败");
        exit(0);
    }
    return head;
}
void SaveStuNodeToFile(Node *head)
{

    FILE *openFile = fopen("./学生信息管理系统.txt", "w"); // 找出最后一个节点
    Node *a = head;
    while (a->next != NULL)
    {
        char buf[1024];
        strcpy(buf, a->next->data.stu_id);
        strcat(buf, " ");
        strcat(buf, a->next->data.stu_name);
        strcat(buf, " ");
        strcat(buf, a->next->data.stu_sex);
        strcat(buf, " ");
        strcat(buf, a->next->data.stu_class);
        strcat(buf, " ");
        char i[10];
        itoa(a->next->data.date.year, i, 10);
        strcat(buf, i);
        strcat(buf, "/");
        itoa(a->next->data.date.month, i, 10);
        strcat(buf, i);
        strcat(buf, "/");
        itoa(a->next->data.date.day, i, 10);
        strcat(buf, i);
        strcat(buf, "\n");
        a = a->next;
        fputs(buf, openFile);
    }
    fclose(openFile);
}
// =============================================================================
void Menu(Node *head, TaskData *S)
{
    printf("欢迎使用学生信息管理系统\n");
    printf("1.增加学生信息       2.删除学生信息       3.根据学号查找       4.根据班级查找\n");
    printf("5.修改学生信息       6.显示学生信息       7.查询已毕业学生     8.查询男女人数\n");
    printf("9.撤回上一步         0.退出系统\n");
    printf("请输入你的选择:");
    int c;
    fflush(stdin);
    scanf("%d", &c);
    switch (c)
    {
    case 0:
        exit(0);
        break;
    case 1:
        Input_StuNode(head, S);
        break;
    case 2:
        Delete_StuNodeByID(head, S);
        break;
    case 3:
        Output_StuNodeByID(head);
        break;
    case 4:
        Statistic_Stu_Class(head);
        break;
    case 5:
        Updata_StuNodeByID(head, S);
        break;
    case 6:
        Output_StuNode(head);
        break;
    case 7:
        Statistic_Stu_Graduation(head);
        break;
    case 8:
        Statistic_Stu_Sex(head);
        break;
    case 9:
        Output_TaskData(S, head);
        break;
    default:
        printf("请重新输入你的选择:");
        break;
    }
}
void Input_StuNode(Node *head, TaskData *S)
{
    int N;
    printf("\n请输入要录入的学生数量：");
    scanf("%d", &N);
    for (int i = 0; i < N; i++)
    {
        printf("\n现在添加第 %d 个学生数据！\n\n", i + 1);
        Node *p = Apply_StuNode();
        p->next = NULL;
        printf("\n学号(纯数字):");
        Input_Stu_ID(p->data.stu_id);
        while (Judge_Id(p->data.stu_id, head))
        {
            printf("学号存在，请重新输入: ");
            Input_Stu_ID(p->data.stu_id);
        }
        printf("\n姓名:");
        Input_Stu_Other(p->data.stu_name);
        printf("\n性别:");
        Input_Stu_Sex(p->data.stu_sex);
        printf("\n班级:");
        Input_Stu_Other(p->data.stu_class);
        printf("\n入学时间(1900~2021年之间,示例 2000 11 11):");

        p->data.date = Input_Stu_Date();
        // 找出最后一个节点
        Node *a = head;
        while (a->next != NULL)
            a = a->next;
        // 将这个节点挂在最后一个节点上
        a->next = p;
        Input_TaskData(S, 0, a, p->data);
    }
    SaveStuNodeToFile(head);
    printf("\n添加成功！\n\n");
}
void Output_StuNode(Node *head)
{
    Node *a = head;
    // 判断链表是否为空
    if (a->next == NULL)
    {
        printf("\n当前没有学生信息数据！\n\n");
        return;
    }
    // 循环输出链表
    printf("\n%-10s %-10s %-10s %-10s %s\n", "学号", "姓名", "性别", "班级", "入学日期");
    while (a->next != NULL)
    {
        printf("%-10s %-10s %-10s %-10s %d/%d/%d\n", a->next->data.stu_id, a->next->data.stu_name, a->next->data.stu_sex, a->next->data.stu_class, a->next->data.date.year, a->next->data.date.month, a->next->data.date.day);
        a = a->next;
    }
    printf("\n");
}
void Output_StuNodeByID(Node *head)
{
    printf("\n请输入要查询的学号(纯数字)：");
    char str[20];
    Input_Stu_ID(str);
    Node *a = head;
    int i = 0;
    while (a->next != NULL)
    {
        if (strcmp(str, a->next->data.stu_id) == 0)
        {
            if (i == 0)
                printf("\n%-10s %-10s %-10s %-10s %s\n", "学号", "姓名", "性别", "班级", "入学日期");
            printf("%-10s %-10s %-10s %-10s %d/%d/%d\n\n", a->next->data.stu_id, a->next->data.stu_name, a->next->data.stu_sex, a->next->data.stu_class, a->next->data.date.year, a->next->data.date.month, a->next->data.date.day);
            i = 1;
        }
        a = a->next;
    }
    if (i == 0)
        printf("该学号不存在，请核对后再输入！\n");
}

void Delete_StuNodeByID(Node *head, TaskData *S)
{
    if (head->next == NULL)
    {
        printf("\n学生信息为空，请输入学生信息后再进行此操作\n");
        return;
    }
    printf("\n请输入需要删除学生的学号:\n");
    char str[20];
    fflush(stdin);
    gets(str);
    Node *a = head;
    while (a->next != NULL)
    {
        if (strcmp(str, a->next->data.stu_id) == 0)
        {
            Input_TaskData(S, 1, a, a->next->data);
            Node *x = a->next->next;
            free(a->next);
            a->next = x;
            printf("\n删除成功\n\n");
            SaveStuNodeToFile(head);
            return;
        }
        a = a->next;
    }
    printf("\n找不到该学生，请检查后重新输入\n\n");
}
void Updata_StuNodeByID(Node *head, TaskData *S)
{
    char str[20];
    printf("\n请输入要修改的学号(纯数字)：");
    Input_Stu_ID(str);
    if (!Judge_Id(str, head))
    {
        printf("学号不存在，请确认后再输入！\n");
        return;
    }
    Node *a = head;
    while (a->next != NULL)
    {
        if (strcmp(str, a->next->data.stu_id) == 0)
        {
            Student stu = a->next->data;
            printf("\n姓名:");
            Input_Stu_Other(a->next->data.stu_name);
            printf("\n性别:");
            Input_Stu_Sex(a->next->data.stu_sex);
            printf("\n班级:");
            Input_Stu_Other(a->next->data.stu_class);
            printf("\n入学时间(1900~2021年之间,示例 2000 11 11):");
            a->next->data.date = Input_Stu_Date();
            // 这里传输的是当前节点a->next
            Input_TaskData(S, 2, a->next, stu);
            printf("\n修改成功！\n\n");
            SaveStuNodeToFile(head);
            return;
        }
        a = a->next;
    }
}
void Statistic_Stu_Class(Node *head)
{
    Node *a = head->next;
    if (a == NULL)
    {
        printf("\n学生信息为空，请添加学生信息后再查询！\n");
        return;
    }
    int x = 0;
    char str[20];
    printf("\n请输入班级号：");
    Input_Stu_Other(str);
    while (a != NULL)
    {
        if (strcmp(str, a->data.stu_class) == 0)
        {
            if (x == 0)
                printf("%-10s %-10s %-10s %-10s %s\n", "学号", "姓名", "性别", "班级", "入学日期");
            printf("%-10s %-10s %-10s %-10s %d/%d/%d\n", a->data.stu_id, a->data.stu_name, a->data.stu_sex, a->data.stu_class, a->data.date.year, a->data.date.month, a->data.date.day);
            x++;
        }
        a = a->next;
    }
    x == 0 ? printf("不存在该班级！\n") : printf("\n班级：%s \n总共有 %d 个学生\n\n", str, x);
}
void Statistic_Stu_Sex(Node *head)
{
    Node *a = head->next;
    if (a == NULL)
    {
        printf("学生信息为空，请添加学生信息后再查询！\n");
        return;
    }
    int x = 0, y = 0;
    char str[10];
    printf("\n请输入性别(男或女)：");
    Input_Stu_Sex(str);
    while (a != NULL)
    {
        if (strcmp(str, a->data.stu_sex) == 0)
            x++;
        y++;
        a = a->next;
    }
    printf("\n总共有 %d 个%s学生\n占全校人数的 %0.1f%%\n\n", x, str, (double)x / y * 100);
}

void Statistic_Stu_Graduation(Node *head)
{
    Node *a = head->next;
    if (a == NULL)
    {
        printf("学生信息为空，请添加学生信息后再查询！\n");
        return;
    }
    int x = 0, y = 0;
    // 获取当前时间
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
    Date s;
    s.year = 1900 + p->tm_year;
    s.month = 1 + p->tm_mon;
    s.day = p->tm_mday;

    while (a != NULL)
    {
        // 入学日期与当前日期之差大于四年的时候，判为毕业
        if (DateInterval(a->data.date, s) >= 1460)
        {
            if (x == 0)
            {
                printf("\n\n已毕业的学生如下:\n");
                printf("%-10s %-10s %-10s %-10s %s\n", "学号", "姓名", "性别", "班级", "入学日期");
            }
            printf("%-10s %-10s %-10s %-10s %d/%d/%d\n", a->data.stu_id, a->data.stu_name, a->data.stu_sex, a->data.stu_class, a->data.date.year, a->data.date.month, a->data.date.day);
            x++;
        }
        y++;
        a = a->next;
    }
    printf("\n\n学生总数：%d\n已毕业人数：%d\n毕业人数占比：%0.1f%%\n", y, x, (double)x / y * 100);
}
// ========================================================================================
int Input_Stu_ID(char *str)
{
    fflush(stdin);
    gets(str);
    if (strspn(str, "0123456789") == strlen(str))
    {
        if (strlen(str) == 0)
        {
            printf("不能为空！请重新输入：");
            Input_Stu_ID(str);
        }
    }
    else
    {
        printf("必须为纯数字！请重新输入：");
        Input_Stu_ID(str);
    }
}

int Judge_Id(char *str, Node *head)
{
    Node *a = head;
    while (a->next != NULL)
    {
        if (strcmp(str, a->next->data.stu_id) == 0)
            return 1;
        a = a->next;
    }
    return 0;
}

void Input_Stu_Other(char *str)
{
    fflush(stdin);
    gets(str);
    if (strlen(str) == 0)
    {
        printf("不能为空，请重新输入：");
        Input_Stu_Other(str);
    }
    int i = 0, x = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ' ')
            x++;
        i++;
    }
    if (i == x)
    {
        printf("不能全为空格，请重新输入：");
        Input_Stu_Other(str);
    }
}
void Input_Stu_Sex(char *str)
{
    fflush(stdin);
    gets(str);
    if (strlen(str) == 0)
    {
        printf("不能为空，请重新输入：");
        Input_Stu_Sex(str);
    }
    else if ((strcmp(str, "男") == 0) || (strcmp(str, "女") == 0))
    {
        return;
    }
    else
    {
        printf("内容不合规(男或女),请重新输入：");
        Input_Stu_Sex(str);
    }
}
Date Input_Stu_Date()
{
    // 获取当前时间
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);

    Date d;
    fflush(stdin);
    char str[20];
    gets(str);
    if (strspn(str, "0123456789 ") == strlen(str))
    {
        int i = 0, x = 0;
        while (str[i] != '\0')
        {
            if (str[i] == ' ')
                x++;
            i++;
        }
        if (x == 2)
        {
            d.year = atoi(strtok(str, " "));
            d.month = atoi(strtok(NULL, " "));
            d.day = atoi(strtok(NULL, " "));
            // 年份要求1900~今年，月份要求1~12月，天数要求符合月份实际天数
            if ((d.year >= 1900 && d.year <= 1900 + p->tm_year) && (d.month >= 1 && d.month <= 12) && (d.day >= 1 && d.day <= Month(d)))
            {
                return d;
            }
            else
            {
                if (d.year < 1900 || d.year > 1900 + p->tm_year)
                    printf("年份应该处于1900~今年，请重新输入：");
                else
                    printf("月份或天数有误，请重新输入：");
                d = Input_Stu_Date();
            }
        }
        else
        {
            printf("日期格式有且仅有两个空格，请重新输入：");
            d = Input_Stu_Date();
        }
    }
    else
    {
        printf("日期不能包含字母或其他特殊字符，请重新输入：");
        d = Input_Stu_Date();
    }
    return d;
}
int Yearday(int year)
{
    int yday;
    if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
        yday = 366;
    else
        yday = 365;
    return yday;
}
int Month(Date d)
{
    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (d.month == 2 && Yearday(d.year) == 366)
        return 29;
    return months[d.month - 1];
}
int EqualDate(Date d, Date s)
{
    int result;
    if (d.year == s.year)
    {
        if (d.month == s.month)
        {
            if (d.day == s.day)
                result = 0;
            else
                result = d.day - s.day;
        }
        else
            result = d.month - s.month;
    }
    else
        result = d.year - s.year;
    return result;
}
int DayOfYear(Date d)
{
    int i, total = 0;
    int months[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (d.month == 2 && Yearday(d.year) == 366)
    {
        months[2] = 29;
    }
    for (i = 1; i < d.month; i++)
        total = total + months[i];
    total = total + d.day;
    return total;
}
int DateInterval(Date d, Date s)
{
    int result, te, ts, total;
    int year, start, end, day;
    int i;
    result = EqualDate(d, s);
    if (result > 0)
    {
        start = s.year;
        end = d.year;
        te = DayOfYear(d);
        ts = DayOfYear(s);
    }
    else if (result < 0)
    {
        start = d.year;
        end = s.year;
        ts = DayOfYear(d);
        te = DayOfYear(s);
    }
    else
        return 0;
    if (start == end)
        return abs(te - ts);
    else
    {
        total = 0;
        for (i = start; i <= end; i++)
        {
            day = Yearday(i);
            if (i == start)
                total = total + day - ts;
            else if (i == end)
                total = total + te;
            else
                total = total + day;
        }
    }
    return total;
}