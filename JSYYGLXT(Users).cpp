//c++
#include <stdlib.h>//学生端用户,老师一样； 
#include <stdio.h>
#include <windows.h>
#include <WinSock.h>
#include <sql.h>
#include <sqlext.h>
#include <Sqltypes.h>
#include <string.h>
#define SNO_LEN 30
#define NAME_LEN 50
#define DEPART_LEN 100
#define SSEX_LEN 5
#define MAX_LEN 255 
//包含附加依赖项，也可以在工程--属性里面设置
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")
char query[150], sno[1000];; //查询语句
SQLHENV kinghenv;        /*环境句柄*/
SQLHDBC kinghdbc;         /*连接句柄*/
SQLHSTMT kinghstmt;  	/*语句句柄*/
SQLRETURN   ret;
typedef struct {
	SQLCHAR UserID[MAX_LEN], StudentUserID[MAX_LEN], Identit[MAX_LEN], UserPassword[MAX_LEN], UserEmail[MAX_LEN], Thing[MAX_LEN], UserSex[MAX_LEN], UserQuestion[MAX_LEN],
		StartTime[MAX_LEN], LastTime[MAX_LEN], Dcapacity[MAX_LEN], MultiMD[MAX_LEN], Time[MAX_LEN], Result[MAX_LEN], UserAnswer[MAX_LEN], Approve[MAX_LEN],
		ClassID[MAX_LEN], Approver[MAX_LEN];
	SQLINTEGER cbUserID, cbIdentit, cbThing, cbStartTime, cbLastTime, cbDcapacity, cbMultiMD, cbTime, cbResult, cbClassID,
		cbStudentUserID, cbUserPassword, cbUserEmail, cbUserSex, cbUserQuestion, cbUserAnswer, cbApprover;
}STUDENT;
STUDENT s;
char userid[50], userpassword[50], UserOldPassWord[100], UserNewPassWord[100], UserEmail[100], UserSex[100], UserQuestion[100], UserAnswer[100], UserID1[MAX_LEN], UserPassword1[MAX_LEN], snp[MAX_LEN];
void inputStu()
{
	printf("请输入一个你的密码：");
	getchar();
	gets(UserOldPassWord);
	printf("\n请输入你的新邮箱：");
	gets(UserEmail);
	printf("\n请输入一个你得性别：");
	gets(UserSex);
	printf("\n请输入你得密保问题：");
	gets(UserQuestion);
	printf("\n请输入密保问题答案：");
	gets(UserAnswer);
}
void printmenu()
{
	//system("cls");
	printf("\n****欢迎进入石大教室预约系统****");
	//printf("\n*      1：登录用户√       *");
	printf("\n*      1：查询空闲教室所有信息√         *");
	printf("\n*      2：填写教室预约申请表√   *");
	printf("\n*      3：更改个人信息√        *");
	printf("\n*      4：查询我已申请的教室预约信息 √      *");
	printf("\n*      5：查询我已完成的审批教室预约的信息 √      *");
	//5测试update 
	printf("\n*      0：退出                 *");
	printf("\n********************************\n");
	printf("请输入：");
}
int main()
{
	int t = 0, k = 0, judge = 0;
	/* Step 1 定义句柄和变量 */

	/*以king开头的表示的是连接KingbaseES的变量*/
	/*以server开头的表示的是连接SQLServer的变量*/

	SQLCHAR dsnname1[] = "s1", logname1[] = "sa", pwd1[] = "123456";  //odbc配置参数 

	SQLCHAR  ClassID[MAX_LEN], Type[MAX_LEN], MultiM[MAX_LEN], Statu[MAX_LEN], Remark[MAX_LEN], UserID[MAX_LEN],
		Identit[MAX_LEN], Thing[MAX_LEN], StartTime[MAX_LEN], LastTime[MAX_LEN], Dcapacity[MAX_LEN], MultiMD[MAX_LEN],
		Time[MAX_LEN], UserPassword[MAX_LEN], Capacity[MAX_LEN], Result[MAX_LEN], StudentUserID[MAX_LEN];   //C 变量 
		//use  ODBC  数据类型 
		//SQLINTEGER  cbAge=0,  cbSno=SQL_NTS,cbSex=SQL_NTS,cbName=SQL_NTS,cbDepart=SQL_NTS;//db cursor


		/* Step 2 初始化环境 */
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &kinghenv);
	//ret=SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&serverhenv);
	ret = SQLSetEnvAttr(kinghenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	//ret=SQLSetEnvAttr(serverhenv,SQL_ATTR_ODBC_VERSION,(void*)SQL_OV_ODBC3, 0);

	/* Step 3 建立连接 */
	ret = SQLAllocHandle(SQL_HANDLE_DBC, kinghenv, &kinghdbc);
	//ret=SQLAllocHandle(SQL_HANDLE_DBC, serverhenv,&serverhdbc);

	//提前配置 odbc:  DSN数据源名  数据库登录名 密码 
	ret = SQLConnect(kinghdbc, (unsigned char *)dsnname1, SQL_NTS, (unsigned char *)logname1, SQL_NTS, (unsigned char *)pwd1, SQL_NTS);
	//ret=SQLConnect(kinghdbc,(SQLCHAR *)"KingbaseES ODBC", SQL_NTS,(SQLCHAR *)"SYSTEM",SQL_NTS, (SQLCHAR *)"MANAGER",SQL_NTS);
	if (!SQL_SUCCEEDED(ret))	/*连接失败时返回错误值*/
	{
		printf("connect1 error...\n");
		return -1;
	}
	else printf("connect1 success...\n");

	//ret=SQLConnect(serverhdbc,(unsigned char *)dsnname2, SQL_NTS,(unsigned char *)logname2,SQL_NTS,(unsigned char *)pwd2,SQL_NTS);
	//ret=SQLConnect(serverhdbc,(unsigned char *)"s2", SQL_NTS,(unsigned char *)"testlogin",SQL_NTS,(unsigned char *)"123",SQL_NTS);
	//ret=SQLConnect(serverhdbc, (SQLCHAR *)"SQLServer", SQL_NTS, (SQLCHAR *)"sa",SQL_NTS,(SQLCHAR *)"sa",SQL_NTS);

	/* Step 4 初始化语句句柄 */
	ret = SQLAllocHandle(SQL_HANDLE_STMT, kinghdbc, &kinghstmt);
	ret = SQLSetStmtAttr(kinghstmt, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)SQL_BIND_BY_COLUMN, SQL_IS_INTEGER);
	do
	{
		int n;
		printf("\n****欢迎进入石大教室预约系统****");
		printf("\n登录请输入1");
		printf("\n注册请输入2\n");
		printf("对不起打扰了，我选择退出系统输入0\n");
		scanf("%d", &n);
		switch (n) {
		case 1:
		{
			getchar();
			printf("请输入你的用户账号：");
			//char userid[50],userpassword;
			char str[1000] = "SELECT * FROM Student_T where StudentUserID=\'";
			gets(userid);
			printf("ID = %s\n", userid);
			printf("请输入你的用户密码:");
			gets(userpassword);
			printf("USERPASSWORD=%s\n", userpassword);
			int i = 0, j = 0;
			SQLCHAR *sql = NULL;
			while (str[i] != '\0')
				i++;
			while (userid[j] != '\0')
				str[i++] = userid[j++]; /* 拼接字符 */
			strcat(str, "\'");
			strcat(str, "and UserPassword=\'");
			strcat(str, userpassword);
			strcat(str, "\'");
			printf("%s", str);
			sql = (SQLCHAR *)str;
			ret = SQLAllocHandle(SQL_HANDLE_STMT, kinghdbc, &kinghstmt);
			ret = SQLExecDirect(kinghstmt, sql, SQL_NTS);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			{
				printf("SQLExecDirect  select  success...\n");
				ret = SQLBindCol(kinghstmt, 1, SQL_C_CHAR, StudentUserID, MAX_LEN, NULL);
				ret = SQLBindCol(kinghstmt, 2, SQL_C_CHAR, UserPassword, MAX_LEN, NULL);
				ret = SQLBindCol(kinghstmt, 3, SQL_C_CHAR, UserEmail, MAX_LEN, NULL);
				ret = SQLBindCol(kinghstmt, 4, SQL_C_CHAR, UserSex, MAX_LEN, NULL);
				ret = SQLBindCol(kinghstmt, 5, SQL_C_CHAR, UserQuestion, MAX_LEN, NULL);
				ret = SQLBindCol(kinghstmt, 6, SQL_C_CHAR, UserAnswer, MAX_LEN, NULL);
				printf("SQLBindCol success...\n");
			}
			else
			{
				printf("SQLExecDirect  select  error...\n");
				return 0;//出现故障即刻停止 
			}
			/* Step 6.2 处理结果集-游标,  逐行SQLFetch  */
			printf("Begin SQLFetch...\n");
			while ((ret = SQLFetch(kinghstmt)) != SQL_NO_DATA_FOUND)
			{
				if (ret == SQL_ERROR)
				{
					printf("没有此用户请重新登录或者注册一个用户"); break;

				}
				else
				{
					printf("您已经登陆成功，欢迎使用教室预约系统！\n"); judge = 1;
				}
			}
			break;
		}
		case 2:
		{
			printf("欢迎注册！请按照下列格式输入你得信息：");
			printf("请输入你的学号，密码，邮箱（没有填写无），性别（隐私填写2,男生填写1,女生填写0），密保问题，密保答案\n");
			ret = SQLAllocHandle(SQL_HANDLE_STMT, kinghdbc, &kinghstmt);
			ret = SQLSetStmtAttr(kinghstmt, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)SQL_BIND_BY_COLUMN, SQL_IS_INTEGER);
			/*Step 5 预处理再执行SQL语句	*/
			/*Step 5  1.预编译带有参数的语句  SQLPrepare、SQLBindParameter*/
			ret = SQLPrepare(kinghstmt, (SQLCHAR *)"insert into Student_T(StudentUserID,UserPassword,UserEmail,UserSex,UserQuestion,UserAnswer) values(?, ?, ?, ?, ?, ?)", SQL_NTS);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			{
				//SQLBindParameter:用于绑定参数的ODBC API函数:执行参数化查询，需要将语句中各个参数对应的变量缓存，与Statement句柄实现绑定。
				ret = SQLBindParameter(kinghstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, StudentUserID, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, UserPassword, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, UserEmail, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, UserSex, 0, NULL);
				//ret=SQLBindParameter(kinghstmt,4,SQL_PARAM_INPUT, SQL_C_LONG,SQL_INTEGER,0,0,&sAge,0,NULL);		
				ret = SQLBindParameter(kinghstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, UserQuestion, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, UserAnswer, 0, NULL);
				printf("SQLBindParameter success...\n");
			}
			else
			{
				printf("SQLBindParameter error...\n");
				break;//错误退出按钮 
			}
			//用户输入 参数 
			/*Step 5  2. 执行预编译后的SQL语句  SQLExecute  多次*/
			while (1)
			{
				printf("input StudentUserID UserPassword UserEmail UserSex UserQuestion UserAnswer:\n");
				scanf("%s%s%s%s%s%s", StudentUserID, UserPassword, UserEmail, UserSex, UserQuestion, UserAnswer);
				printf("you input :%s %s %s %s %s %s \n", StudentUserID, UserPassword, UserEmail, UserSex, UserQuestion, UserAnswer);
				ret = SQLExecute(kinghstmt);
				if (ret == SQL_ERROR)
				{
					printf("insert error...\n");
					break;
				}
				else if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
					printf("insert success...\n");
				else printf("insert ???...\n"); break;
				printf("go on ?");
				scanf("%d", &k);//1 0
				if (k == 0) break;
			}
			printf("您已经注册成功！请重新登录，谢谢！\n");
			break;
		}
		case 0:
			return 0;
		default:
			printf("请输入正确的选项\n");
		}
	} while (judge == 0);
	do
	{
		printmenu();  //主菜单 
		scanf("%d", &t);
		switch (t)
		{
		case 1:
		{
			ret = SQLAllocHandle(SQL_HANDLE_STMT, kinghdbc, &kinghstmt);

			/*Step 5 直接执行SQL语句	*/
			ret = SQLExecDirect(kinghstmt, (SQLCHAR *)"SELECT * FROM  Class_T where Statu=1", SQL_NTS);

			/* Step 6.1 处理结果集-游标，将数据缓冲（C变量）绑定到结果集的各列 SQLBindCol  */
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			{
				printf("SQLExecDirect  select  success...\n");
				ret = SQLBindCol(kinghstmt, 1, SQL_C_CHAR, ClassID, MAX_LEN, NULL);
				ret = SQLBindCol(kinghstmt, 2, SQL_C_CHAR, Capacity, MAX_LEN, NULL);
				ret = SQLBindCol(kinghstmt, 3, SQL_C_CHAR, Type, MAX_LEN, NULL);
				ret = SQLBindCol(kinghstmt, 4, SQL_C_CHAR, MultiM, MAX_LEN, NULL);
				ret = SQLBindCol(kinghstmt, 5, SQL_C_CHAR, Statu, MAX_LEN, NULL);
				printf("SQLBindCol success...\n");
			}
			else
			{
				printf("SQLExecDirect  select  error...\n");
				break;//出现故障即刻停止 
			}
			/* Step 6.2 处理结果集-游标,  逐行SQLFetch  */
			printf("Begin SQLFetch...\n");
			while ((ret = SQLFetch(kinghstmt)) != SQL_NO_DATA_FOUND)
			{
				if (ret == SQL_ERROR)	printf("Fetch error\n");
				else
				{
					printf("教室号码：%s  容量：%s  类型：%s  是否配备多媒体:%s 状态：%s \n", ClassID, Capacity, Type, MultiM, Statu, Remark);
				}
			}
			break;
		}
		case 2:
		{
			ret = SQLAllocHandle(SQL_HANDLE_STMT, kinghdbc, &kinghstmt);
			ret = SQLSetStmtAttr(kinghstmt, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)SQL_BIND_BY_COLUMN, SQL_IS_INTEGER);
			/*Step 5 预处理再执行SQL语句	*/
			/*Step 5  1.预编译带有参数的语句  SQLPrepare、SQLBindParameter*/
			ret = SQLPrepare(kinghstmt, (SQLCHAR *)"insert into ApplyC_T(UserID,Identit,Thing,StartTime,LastTime,Dcapacity,MultiMD,Time,Result) values(?, ?, ?, ?, ?, ?, ?, ?, ?)", SQL_NTS);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			{
				//SQLBindParameter:用于绑定参数的ODBC API函数:执行参数化查询，需要将语句中各个参数对应的变量缓存，与Statement句柄实现绑定。
				ret = SQLBindParameter(kinghstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, UserID, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, Identit, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, Thing, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, StartTime, 0, NULL);
				//ret=SQLBindParameter(kinghstmt,4,SQL_PARAM_INPUT, SQL_C_LONG,SQL_INTEGER,0,0,&sAge,0,NULL);		
				ret = SQLBindParameter(kinghstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, LastTime, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, Dcapacity, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, MultiMD, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, Time, 0, NULL);
				ret = SQLBindParameter(kinghstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_LEN, 0, Result, 0, NULL);
				printf("SQLBindParameter success...\n");
			}
			else
			{
				printf("SQLBindParameter error...\n");
				break;//错误退出按钮 
			}
			//用户输入 参数 
			/*Step 5  2. 执行预编译后的SQL语句  SQLExecute  多次*/
			while (1)
			{
				printf("input UserID Identit Thing StartTime LastTime Dcapacity MultiMD Time Result(0):\n");
				scanf("%s%s%s%s%s%s%s%s%s", UserID, Identit, Thing, StartTime, LastTime, Dcapacity, MultiMD, Time, Result);
				printf("you input :%s %s %s %s %s %s %s %s %s\n", UserID, Identit, Thing, StartTime, LastTime, Dcapacity, MultiMD, Time, Result);
				ret = SQLExecute(kinghstmt);
				if (ret == SQL_ERROR)
				{
					printf("insert error...\n");
					break;
				}
				else if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
					printf("insert success...\n");
				else printf("insert ???...\n"); break;
				getchar();
				printf("go on ?");
				scanf("%d", &k);//1 0
				if (k == 0) break;
			}
			break;
		}
		case 3:
		{
			inputStu();
			int i = 0, j = 0;
			SQLCHAR *sql = NULL;
			char Userpass[100];
			printf("请输入你的新密码！");
			scanf("%s", Userpass);
			//update
			//char id1[100];
			//char id2[100];
			char str[1000] = "update Student_T  set UserPassword=\'";
			strcat(str, Userpass);
			strcat(str, "\'");
			//scanf("%s",id1);
			//scanf("%s",id2);
			strcat(str, ",UserEmail=\'");
			strcat(str, UserEmail);
			strcat(str, "\'");
			strcat(str, ",UserSex=\'");
			strcat(str, UserSex);
			strcat(str, "\'");
			strcat(str, ",UserQuestion=\'");
			strcat(str, UserQuestion);
			strcat(str, "\'");
			strcat(str, ",UserAnswer=\'");
			strcat(str, UserAnswer);
			strcat(str, "\' where UserPassword=\'");
			strcat(str, UserOldPassWord);
			strcat(str, "\'");
			strcat(str, "and StudentUserID=\'");
			strcat(str, userid);
			strcat(str, "\'");
			printf("\nupdate:%s\n", str);
			sql = (SQLCHAR *)str;
			s.cbStudentUserID = SQL_NTS;
			s.cbUserPassword = SQL_NTS;
			s.cbUserEmail = SQL_NTS;
			s.cbUserSex = SQL_NTS;
			s.cbUserQuestion = SQL_NTS;
			s.cbUserAnswer = SQL_NTS;
			ret = SQLAllocHandle(SQL_HANDLE_STMT, kinghdbc, &kinghstmt);
			ret = SQLExecDirect(kinghstmt, sql, SQL_NTS);
			/*char strselect[1000]="select * from Student_T";
			sql = (SQLCHAR *)strselect;
			ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);
			ret = SQLExecDirect(kinghstmt, sql, SQL_NTS);
				if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
				{
					printf("SQLExecDirect  select  success...\n");
					ret = SQLBindCol(kinghstmt, 1, SQL_C_CHAR, s.StudentUserID, MAX_LEN, (SQLLEN*)&s.cbStudentUserID);
					ret = SQLBindCol(kinghstmt, 2, SQL_C_CHAR, s.UserPassword, MAX_LEN, (SQLLEN*)&s.cbUserPassword);
					ret = SQLBindCol(kinghstmt, 3, SQL_C_CHAR, s.UserEmail, MAX_LEN, (SQLLEN*)&s.cbUserEmail);
					ret = SQLBindCol(kinghstmt, 4, SQL_C_CHAR, s.UserSex, MAX_LEN, (SQLLEN*)&s.cbUserSex);
					//ret = SQLBindCol(kinghstmt, 4, SQL_C_LONG, s.sage, 0, &s.cbsage);
					ret = SQLBindCol(kinghstmt, 5, SQL_C_CHAR, s.UserQuestion, MAX_LEN, (SQLLEN*)&s.cbUserQuestion);
					ret = SQLBindCol(kinghstmt, 6, SQL_C_CHAR, s.UserAnswer, MAX_LEN, (SQLLEN*)&s.cbUserAnswer);
					printf("SQLBindCol success...\n");
				}
				else
				{
						printf("SQLExecDirect  select  error...\n");
						break;//出现故障即刻停止
				}
				printf("Begin SQLFetch...\n");
				while ((ret=SQLFetch(kinghstmt))!=SQL_NO_DATA_FOUND)
						{
							if(ret==SQL_ERROR)
							{
					printf("Fetch error\n");
					break;
				}
							else
							{
								printf("%s  %s  %s  %s %s %s\n",s.StudentUserID,s.UserPassword,s.UserEmail,s.UserSex,s.UserQuestion,s.UserAnswer);
							}
						}*/
			break;
		}
		case 4: {
			getchar();
			//printf("\n请输入用户名：");
			//gets(snp);
			printf("ID = %s\n", userid);
			int i = 0, j = 0;
			SQLCHAR *sql = NULL;
			char str[1000] = "SELECT * FROM ApplyC_T where UserID=\'";
			while (str[i] != '\0')
				i++;
			while (userid[j] != '\0')
				str[i++] = userid[j++]; /* 拼接字符 */
			strcat(str, "'");
			sql = (SQLCHAR *)str;
			printf("get()函数\n");
			for (i = 0; i < strlen(str); i++)
			{
				printf("%c", str[i]);
			}
			int count = 1;
			s.cbUserID = SQL_NTS;
			s.cbIdentit = SQL_NTS;
			s.cbThing = SQL_NTS;
			s.cbStartTime = SQL_NTS;
			s.cbLastTime = SQL_NTS;
			s.cbDcapacity = SQL_NTS;
			s.cbMultiMD = SQL_NTS;
			s.cbTime = SQL_NTS;
			s.cbResult = SQL_NTS;
			ret = SQLAllocHandle(SQL_HANDLE_STMT, kinghdbc, &kinghstmt);
			ret = SQLExecDirect(kinghstmt, sql, SQL_NTS);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			{
				printf("SQLExecDirect  select  success...\n");
				ret = SQLBindCol(kinghstmt, 1, SQL_C_CHAR, s.UserID, MAX_LEN, (SQLLEN*)&s.cbUserID);
				ret = SQLBindCol(kinghstmt, 2, SQL_C_CHAR, s.Identit, MAX_LEN, (SQLLEN*)&s.cbIdentit);
				ret = SQLBindCol(kinghstmt, 3, SQL_C_CHAR, s.Thing, MAX_LEN, (SQLLEN*)&s.cbThing);
				ret = SQLBindCol(kinghstmt, 4, SQL_C_CHAR, s.StartTime, MAX_LEN, (SQLLEN*)&s.cbStartTime);
				//ret = SQLBindCol(kinghstmt, 4, SQL_C_LONG, s.sage, 0, &s.cbsage);
				ret = SQLBindCol(kinghstmt, 5, SQL_C_CHAR, s.LastTime, MAX_LEN, (SQLLEN*)&s.cbLastTime);
				ret = SQLBindCol(kinghstmt, 6, SQL_C_CHAR, s.Dcapacity, MAX_LEN, (SQLLEN*)&s.cbDcapacity);
				ret = SQLBindCol(kinghstmt, 7, SQL_C_CHAR, s.MultiMD, MAX_LEN, (SQLLEN*)&s.cbMultiMD);
				ret = SQLBindCol(kinghstmt, 8, SQL_C_CHAR, s.Time, MAX_LEN, (SQLLEN*)&s.cbTime);
				ret = SQLBindCol(kinghstmt, 9, SQL_C_CHAR, s.Result, MAX_LEN, (SQLLEN*)&s.cbResult);
				printf("SQLBindCol success...\n");
			}
			else
			{
				printf("SQLExecDirect  select  error...\n");
				break;//出现故障即刻停止
			}
			/***处理结果**/
			printf("Begin SQLFetch...\n");
			while ((ret = SQLFetch(kinghstmt)) != SQL_NO_DATA_FOUND)
			{
				if (ret == SQL_ERROR)	printf("Fetch error\n");
				else
				{
					printf("%s  %s  %s  %s %s %s %s %s %s\n", s.UserID, s.Identit, s.Thing, s.StartTime, s.LastTime, s.Dcapacity, s.MultiMD, s.Time, s.Result);
				}
			}
			break;
		}
		case 5:
		{
			SQLCHAR *sql = NULL;
			int i = 0, j = 0;
			char str[1000] = "SELECT * FROM ReserveC_T where UserID=\'";
			while (str[i] != '\0')
				i++;
			while (userid[j] != '\0')
				str[i++] = userid[j++]; /* 拼接字符 */
			strcat(str, "'");
			sql = (SQLCHAR *)str;
			printf("get()函数\n");
			for (i = 0; i < strlen(str); i++)
			{
				printf("%c", str[i]);
			}
			s.cbUserID = SQL_NTS;
			s.cbClassID = SQL_NTS;
			s.cbApprover = SQL_NTS;
			s.cbTime = SQL_NTS;
			s.cbResult = SQL_NTS;
			ret = SQLAllocHandle(SQL_HANDLE_STMT, kinghdbc, &kinghstmt);
			ret = SQLExecDirect(kinghstmt, sql, SQL_NTS);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			{
				printf("SQLExecDirect  select  success...\n");
				ret = SQLBindCol(kinghstmt, 1, SQL_C_CHAR, s.UserID, MAX_LEN, (SQLLEN*)&s.cbUserID);
				ret = SQLBindCol(kinghstmt, 2, SQL_C_CHAR, s.ClassID, MAX_LEN, (SQLLEN*)&s.cbClassID);
				ret = SQLBindCol(kinghstmt, 3, SQL_C_CHAR, s.Approver, MAX_LEN, (SQLLEN*)&s.cbApprover);
				ret = SQLBindCol(kinghstmt, 4, SQL_C_CHAR, s.Time, MAX_LEN, (SQLLEN*)&s.cbTime);
				//ret = SQLBindCol(kinghstmt, 4, SQL_C_LONG, s.sage, 0, &s.cbsage);
				ret = SQLBindCol(kinghstmt, 5, SQL_C_CHAR, s.Result, MAX_LEN, (SQLLEN*)&s.cbResult);
				printf("SQLBindCol success...\n");
			}
			else
			{
				printf("SQLExecDirect  select  error...\n");
				break;//出现故障即刻停止
			}
			/***处理结果**/
			printf("Begin SQLFetch...\n");
			while ((ret = SQLFetch(kinghstmt)) != SQL_NO_DATA_FOUND)
			{
				if (ret == SQL_ERROR)	printf("Fetch error\n");
				else
				{
					printf("UserID=%s  ClassID=%s  Approver=%s  Time=%s Result=%s \n", s.UserID, s.ClassID, s.Approver, s.Time, s.Result);
				}
			}
			break;
		}

		case 0:
			/* Step 7 中止处理*/
			SQLFreeHandle(SQL_HANDLE_STMT, kinghstmt);
			SQLDisconnect(kinghdbc);
			SQLFreeHandle(SQL_HANDLE_DBC, kinghdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, kinghenv);
			printf("SQLFree.\n ");
			return 0;
		default:printf("请输入正确的选项\n");
		}
	} while (1);
	return 0;
}
