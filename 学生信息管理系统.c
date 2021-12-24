#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <io.h>
// ��ѧ����
typedef struct date
{
    int year, month, day;
} Date;
// ѧ����Ϣ
typedef struct student
{
    char stu_id[20];    //ѧ��
    char stu_name[20];  //����
    char stu_sex[4];    //�Ա�
    char stu_class[20]; //�༶
    Date date;          //��ѧʱ��
} Student;
// ѧ����Ϣ����
typedef struct node
{
    Student data;
    struct node *next; //��һ���ڵ�
} Node;
//��������,����0,ɾ��1,�޸�2
typedef struct task
{
    int operate; //��ִ�еĲ���
    Node *last;  //��¼��һ���ڵ�
    Student data;
} Task;
// ��¼���������˳��ջ
typedef struct taskdata
{
    Task task[10];
    int top; //ջ����ַ
} TaskData;

// ��ʼ��˳��ջ
TaskData *Revoke_init();
// ����������ջ
void Input_TaskData(TaskData *S, int operate, Node *last, Student data);
// ���������ջ
void Output_TaskData(TaskData *S, Node *head);
// ��ʼ��ѧ����Ϣ����
Node *Stu_init(TaskData *S);
// ����ѧ����Ϣ����ڵ��ڴ�ռ�
Node *Apply_StuNode();
// �˵�
void Menu(Node *head, TaskData *S);
// ����ѧ����Ϣ
void Input_StuNode(Node *head, TaskData *S);
// ����ѧ��
int Input_Stu_ID(char *str);
// �ж�ѧ���Ƿ����
int Judge_Id(char *str, Node *head);
// �����Ա��л�Ů
void Input_Stu_Sex(char *str);
// ������ѧ���ڣ�����޶���1900~2021�꣬
Date Input_Stu_Date();
// �����ַ���������Ϊ��
void Input_Stu_Other(char *str);
// ����ѧ�Ų�ѯѧ����Ϣ
void Output_StuNodeByID(Node *head);
// ɾ��ѧ���ڵ�
void Delete_StuNodeByID(Node *head, TaskData *S);
// �޸�ѧ����Ϣ
void Updata_StuNodeByID(Node *head, TaskData *S);
// ���ѧ���ڵ���Ϣ
void Output_StuNode(Node *head);
// ͳ�ư༶���� ����ѯѧ����Ϣ
void Statistic_Stu_Class(Node *head);
// ͳ����Ů����
void Statistic_Stu_Sex(Node *head);
// ͳ�Ʋ���ѯ�ѱ�ҵ��ѧ��
void Statistic_Stu_Graduation(Node *head);
// ����ѧ���ڵ���Ϣ���ĵ�
void SaveStuNodeToFile(Node *head);
// �����Ƿ�Ϊ����
int Yearday(int year);
// �����·�����
int Month(Date d);
// �ж������Ƿ����
int EqualDate(Date d, Date s);
// ��ȡһ�����Ѷȹ�������
int DayOfYear(Date d);
// �������ڼ������
int DateInterval(Date d, Date s);
// ===============================================�������====================================
int main()
{
    //�������ܳ�ʼ��
    TaskData *S = Revoke_init();
    // ����ѧ����Ϣ����ͷָ�룬ָ���һ���ڵ�
    Node *head = Stu_init(S);
    // ѭ�������˵�
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
    // ����ռ�
    S = (TaskData *)malloc(sizeof(TaskData));
    if (S == NULL)
    {
        printf("�����ڴ�ռ�ʧ��");
        exit(0);
    }
    // ��ʼ��ջ��λ��
    S->top = -1;
    return S;
}
void Input_TaskData(TaskData *S, int operate, Node *last, Student data)
{
    Task a;
    a.data = data;
    a.last = last;
    a.operate = operate;
    // ջ������+1
    S->top = (S->top + 1) % 10;
    // ������ջ
    S->task[(S->top) % 10] = a;
}
void Output_TaskData(TaskData *S, Node *head)
{
    Node *a = head;
    if (S->top == -1)
    {
        printf("\nû�п��Գ��������ݣ�\n\n");
        return;
    }
    // ��ȡ��Ҫ�����Ĳ���
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
                printf("\n�����ɹ���\n\n");
                return;
            }
            a = a->next;
        }
    }
        printf("\n����ʧ�ܣ��޷�������\n\n");
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
            printf("\n�����ɹ���\n\n");
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
                    printf("\n�����ɹ���\n\n");
                    return;
                }
                a = a->next;
            }
        }
    }
        printf("\n����ʧ�ܣ��޷�������\n\n");
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
                printf("\n�����ɹ���\n\n");
                return;
            }
            a = a->next;
        }
    }
        printf("\n����ʧ�ܣ��޷�������\n\n");
        break;
    default:
        printf("\n����ʧ�ܣ��޷�������\n\n");
        break;
    }
}
Node *Stu_init(TaskData *S)
{
    Node *head;
    head = Apply_StuNode();
    // ��ʼ��ͷ���
    head->next = NULL;
    if (access("./ѧ����Ϣ����ϵͳ.txt", 0))
    {
        printf("\n��ӭʹ��ѧ����Ϣ����ϵͳ\n\n");
        printf("��ϵͳ�����Ĺ����У�\n");
        printf("��ӡ��޸ġ�ɾ������ѯѧ����Ϣ\n");
        printf("ͳ�ư༶������ͳ����Ů��������ѯ��ҵ�����ȹ���\n\n");
        printf("\n�������ǵ�һ��ʹ�ã���Ҫ��¼��ѧ����Ϣ\n\n");
        system("PAUSE");
        Input_StuNode(head, S);
        system("CLS");
        printf("\n��Ϣ¼��ɹ������Ǹո�¼���ѧ����Ϣ��\n");
        Output_StuNode(head);
        printf("\n\n�����������ϵͳ\n");
        system("PAUSE");
    }
    else
    {
        // ��ȡ�ļ����������
        // �1024���ֽ�
        char buf[1024];
        // �洢ÿ�еĳ���
        int line_len = 0;
        // �����·���µ�a.txt�ļ�
        FILE *openFile = fopen("./ѧ����Ϣ����ϵͳ.txt", "r");
        // ѭ�����ж�ȡ
        while (fgets(buf, 1024, openFile))
        {
            // ��ȡ�ļ�����
            line_len = strlen(buf);
            // �ų����кͻس�
            while ('\n' == buf[line_len - 1] || '\r' == buf[line_len - 1])
            {
                buf[line_len - 1] = '\0';
                //ȥ�����к󣬳��ȼ�һ��Ȼ���ж��Ƿ�����Ϊ��
                line_len--;
            }
            if (0 == line_len)
                continue;
            // �ֽ��ַ��������ո�ֽ�
            Node *p = Apply_StuNode();
            p->next = NULL;
            strcpy(p->data.stu_id, strtok(buf, " "));
            strcpy(p->data.stu_name, strtok(NULL, " "));
            strcpy(p->data.stu_sex, strtok(NULL, " "));
            strcpy(p->data.stu_class, strtok(NULL, " "));
            // ��ȡ���ڣ�ת��Ϊ����
            p->data.date.year = atoi(strtok(NULL, "/"));
            p->data.date.month = atoi(strtok(NULL, "/"));
            p->data.date.day = atoi(strtok(NULL, "/"));
            // ѭ�������һ���ڵ�
            Node *a = head;
            while (a->next != NULL)
                a = a->next;
            // ������ڵ�������һ���ڵ���
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
        printf("�����ڴ�ռ�ʧ��");
        exit(0);
    }
    return head;
}
void SaveStuNodeToFile(Node *head)
{

    FILE *openFile = fopen("./ѧ����Ϣ����ϵͳ.txt", "w"); // �ҳ����һ���ڵ�
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
    printf("��ӭʹ��ѧ����Ϣ����ϵͳ\n");
    printf("1.����ѧ����Ϣ       2.ɾ��ѧ����Ϣ       3.����ѧ�Ų���       4.���ݰ༶����\n");
    printf("5.�޸�ѧ����Ϣ       6.��ʾѧ����Ϣ       7.��ѯ�ѱ�ҵѧ��     8.��ѯ��Ů����\n");
    printf("9.������һ��         0.�˳�ϵͳ\n");
    printf("���������ѡ��:");
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
        printf("�������������ѡ��:");
        break;
    }
}
void Input_StuNode(Node *head, TaskData *S)
{
    int N;
    printf("\n������Ҫ¼���ѧ��������");
    scanf("%d", &N);
    for (int i = 0; i < N; i++)
    {
        printf("\n������ӵ� %d ��ѧ�����ݣ�\n\n", i + 1);
        Node *p = Apply_StuNode();
        p->next = NULL;
        printf("\nѧ��(������):");
        Input_Stu_ID(p->data.stu_id);
        while (Judge_Id(p->data.stu_id, head))
        {
            printf("ѧ�Ŵ��ڣ�����������: ");
            Input_Stu_ID(p->data.stu_id);
        }
        printf("\n����:");
        Input_Stu_Other(p->data.stu_name);
        printf("\n�Ա�:");
        Input_Stu_Sex(p->data.stu_sex);
        printf("\n�༶:");
        Input_Stu_Other(p->data.stu_class);
        printf("\n��ѧʱ��(1900~2021��֮��,ʾ�� 2000 11 11):");

        p->data.date = Input_Stu_Date();
        // �ҳ����һ���ڵ�
        Node *a = head;
        while (a->next != NULL)
            a = a->next;
        // ������ڵ�������һ���ڵ���
        a->next = p;
        Input_TaskData(S, 0, a, p->data);
    }
    SaveStuNodeToFile(head);
    printf("\n��ӳɹ���\n\n");
}
void Output_StuNode(Node *head)
{
    Node *a = head;
    // �ж������Ƿ�Ϊ��
    if (a->next == NULL)
    {
        printf("\n��ǰû��ѧ����Ϣ���ݣ�\n\n");
        return;
    }
    // ѭ���������
    printf("\n%-10s %-10s %-10s %-10s %s\n", "ѧ��", "����", "�Ա�", "�༶", "��ѧ����");
    while (a->next != NULL)
    {
        printf("%-10s %-10s %-10s %-10s %d/%d/%d\n", a->next->data.stu_id, a->next->data.stu_name, a->next->data.stu_sex, a->next->data.stu_class, a->next->data.date.year, a->next->data.date.month, a->next->data.date.day);
        a = a->next;
    }
    printf("\n");
}
void Output_StuNodeByID(Node *head)
{
    printf("\n������Ҫ��ѯ��ѧ��(������)��");
    char str[20];
    Input_Stu_ID(str);
    Node *a = head;
    int i = 0;
    while (a->next != NULL)
    {
        if (strcmp(str, a->next->data.stu_id) == 0)
        {
            if (i == 0)
                printf("\n%-10s %-10s %-10s %-10s %s\n", "ѧ��", "����", "�Ա�", "�༶", "��ѧ����");
            printf("%-10s %-10s %-10s %-10s %d/%d/%d\n\n", a->next->data.stu_id, a->next->data.stu_name, a->next->data.stu_sex, a->next->data.stu_class, a->next->data.date.year, a->next->data.date.month, a->next->data.date.day);
            i = 1;
        }
        a = a->next;
    }
    if (i == 0)
        printf("��ѧ�Ų����ڣ���˶Ժ������룡\n");
}

void Delete_StuNodeByID(Node *head, TaskData *S)
{
    if (head->next == NULL)
    {
        printf("\nѧ����ϢΪ�գ�������ѧ����Ϣ���ٽ��д˲���\n");
        return;
    }
    printf("\n��������Ҫɾ��ѧ����ѧ��:\n");
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
            printf("\nɾ���ɹ�\n\n");
            SaveStuNodeToFile(head);
            return;
        }
        a = a->next;
    }
    printf("\n�Ҳ�����ѧ�����������������\n\n");
}
void Updata_StuNodeByID(Node *head, TaskData *S)
{
    char str[20];
    printf("\n������Ҫ�޸ĵ�ѧ��(������)��");
    Input_Stu_ID(str);
    if (!Judge_Id(str, head))
    {
        printf("ѧ�Ų����ڣ���ȷ�Ϻ������룡\n");
        return;
    }
    Node *a = head;
    while (a->next != NULL)
    {
        if (strcmp(str, a->next->data.stu_id) == 0)
        {
            Student stu = a->next->data;
            printf("\n����:");
            Input_Stu_Other(a->next->data.stu_name);
            printf("\n�Ա�:");
            Input_Stu_Sex(a->next->data.stu_sex);
            printf("\n�༶:");
            Input_Stu_Other(a->next->data.stu_class);
            printf("\n��ѧʱ��(1900~2021��֮��,ʾ�� 2000 11 11):");
            a->next->data.date = Input_Stu_Date();
            // ���ﴫ����ǵ�ǰ�ڵ�a->next
            Input_TaskData(S, 2, a->next, stu);
            printf("\n�޸ĳɹ���\n\n");
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
        printf("\nѧ����ϢΪ�գ������ѧ����Ϣ���ٲ�ѯ��\n");
        return;
    }
    int x = 0;
    char str[20];
    printf("\n������༶�ţ�");
    Input_Stu_Other(str);
    while (a != NULL)
    {
        if (strcmp(str, a->data.stu_class) == 0)
        {
            if (x == 0)
                printf("%-10s %-10s %-10s %-10s %s\n", "ѧ��", "����", "�Ա�", "�༶", "��ѧ����");
            printf("%-10s %-10s %-10s %-10s %d/%d/%d\n", a->data.stu_id, a->data.stu_name, a->data.stu_sex, a->data.stu_class, a->data.date.year, a->data.date.month, a->data.date.day);
            x++;
        }
        a = a->next;
    }
    x == 0 ? printf("�����ڸð༶��\n") : printf("\n�༶��%s \n�ܹ��� %d ��ѧ��\n\n", str, x);
}
void Statistic_Stu_Sex(Node *head)
{
    Node *a = head->next;
    if (a == NULL)
    {
        printf("ѧ����ϢΪ�գ������ѧ����Ϣ���ٲ�ѯ��\n");
        return;
    }
    int x = 0, y = 0;
    char str[10];
    printf("\n�������Ա�(�л�Ů)��");
    Input_Stu_Sex(str);
    while (a != NULL)
    {
        if (strcmp(str, a->data.stu_sex) == 0)
            x++;
        y++;
        a = a->next;
    }
    printf("\n�ܹ��� %d ��%sѧ��\nռȫУ������ %0.1f%%\n\n", x, str, (double)x / y * 100);
}

void Statistic_Stu_Graduation(Node *head)
{
    Node *a = head->next;
    if (a == NULL)
    {
        printf("ѧ����ϢΪ�գ������ѧ����Ϣ���ٲ�ѯ��\n");
        return;
    }
    int x = 0, y = 0;
    // ��ȡ��ǰʱ��
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
        // ��ѧ�����뵱ǰ����֮����������ʱ����Ϊ��ҵ
        if (DateInterval(a->data.date, s) >= 1460)
        {
            if (x == 0)
            {
                printf("\n\n�ѱ�ҵ��ѧ������:\n");
                printf("%-10s %-10s %-10s %-10s %s\n", "ѧ��", "����", "�Ա�", "�༶", "��ѧ����");
            }
            printf("%-10s %-10s %-10s %-10s %d/%d/%d\n", a->data.stu_id, a->data.stu_name, a->data.stu_sex, a->data.stu_class, a->data.date.year, a->data.date.month, a->data.date.day);
            x++;
        }
        y++;
        a = a->next;
    }
    printf("\n\nѧ��������%d\n�ѱ�ҵ������%d\n��ҵ����ռ�ȣ�%0.1f%%\n", y, x, (double)x / y * 100);
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
            printf("����Ϊ�գ����������룺");
            Input_Stu_ID(str);
        }
    }
    else
    {
        printf("����Ϊ�����֣����������룺");
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
        printf("����Ϊ�գ����������룺");
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
        printf("����ȫΪ�ո����������룺");
        Input_Stu_Other(str);
    }
}
void Input_Stu_Sex(char *str)
{
    fflush(stdin);
    gets(str);
    if (strlen(str) == 0)
    {
        printf("����Ϊ�գ����������룺");
        Input_Stu_Sex(str);
    }
    else if ((strcmp(str, "��") == 0) || (strcmp(str, "Ů") == 0))
    {
        return;
    }
    else
    {
        printf("���ݲ��Ϲ�(�л�Ů),���������룺");
        Input_Stu_Sex(str);
    }
}
Date Input_Stu_Date()
{
    // ��ȡ��ǰʱ��
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
            // ���Ҫ��1900~���꣬�·�Ҫ��1~12�£�����Ҫ������·�ʵ������
            if ((d.year >= 1900 && d.year <= 1900 + p->tm_year) && (d.month >= 1 && d.month <= 12) && (d.day >= 1 && d.day <= Month(d)))
            {
                return d;
            }
            else
            {
                if (d.year < 1900 || d.year > 1900 + p->tm_year)
                    printf("���Ӧ�ô���1900~���꣬���������룺");
                else
                    printf("�·ݻ������������������룺");
                d = Input_Stu_Date();
            }
        }
        else
        {
            printf("���ڸ�ʽ���ҽ��������ո����������룺");
            d = Input_Stu_Date();
        }
    }
    else
    {
        printf("���ڲ��ܰ�����ĸ�����������ַ������������룺");
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