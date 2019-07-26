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
char query[150],sno[1000];; //查询语句
int J;//input()模块判断老师还是学生 
SQLHENV kinghenv;        /*环境句柄*/
SQLHDBC kinghdbc;         /*连接句柄*/
SQLHSTMT kinghstmt;  	/*语句句柄*/
SQLRETURN   ret;
typedef struct {
	SQLCHAR UserID[MAX_LEN],StudentUserID[MAX_LEN], Identit[MAX_LEN],UserPassword[MAX_LEN],UserEmail[MAX_LEN],Thing[MAX_LEN],UserSex[MAX_LEN],UserQuestion[MAX_LEN],
	StartTime[MAX_LEN], LastTime[MAX_LEN],Dcapacity[MAX_LEN],MultiMD[MAX_LEN],Time[MAX_LEN],Result[MAX_LEN],UserAnswer[MAX_LEN],Approve[MAX_LEN],
	ClassID[MAX_LEN],Approver[MAX_LEN],TeacherUserID[MAX_LEN],Post[MAX_LEN];
	SQLINTEGER cbUserID, cbIdentit, cbThing, cbStartTime, cbLastTime,cbDcapacity,cbMultiMD,cbTime,cbResult,cbClassID,
	cbStudentUserID,cbUserPassword,cbUserEmail,cbUserSex,cbUserQuestion,cbUserAnswer,cbApprover,cbTeacherUserID,cbPost;
}STUDENT;
STUDENT s;
char UserPost[50],userid[50],userpassword[50],UserOldPassWord[100],UserNewPassWord[100], UserEmail[100], UserSex[100], UserQuestion[100],UserAnswer[100],UserID1[MAX_LEN],UserPassword1[MAX_LEN],snp[MAX_LEN];
void inputStu()
{
	getchar();
	printf("\n请输入新邮箱：");
	gets(UserEmail);
	printf("\n请输入一个你得性别：");
	gets(UserSex);
	printf("\n请输入你得密保问题：");
	gets(UserQuestion);
	printf("\n请输入密保问题答案：");
	gets(UserAnswer);
	if(J==1)
	{
		printf("\n请输入老师的职位：");
		gets(UserPost);
	}
	
}
void printmenu()
{
	//system("cls");
	printf("\n****欢迎进入石大教室预约系统（管理员）****");
	//printf("\n*      1：登录用户√       *");
	printf("\n*      1：查询所有学生信息√         *");
	printf("\n*      2：查询所有老师信息√   *");
	printf("\n*      3：查询所有教室预约申请表√   *");
	printf("\n*      4：更改/删除学生信息√        *");
	printf("\n*      5：更改/删除老师信息√        *");
	printf("\n*      6：审批用户教室预约信息表 √      *");
	//5测试update 
	printf("\n*      7：增加考试信息√      *");
	printf("\n*      0：退出                 *");
	printf("\n********************************\n");
	printf("请输入：");
} 
int main()
{	int t=0,k=0,judge=0; 
	/* Step 1 定义句柄和变量 */

	/*以king开头的表示的是连接KingbaseES的变量*/
	/*以server开头的表示的是连接SQLServer的变量*/
	
	SQLCHAR dsnname1[]="s1",logname1[]="sa",pwd1[]="123456";  //odbc配置参数 
	
	SQLCHAR  ClassID[MAX_LEN],Type[MAX_LEN],MultiM[MAX_LEN],Statu[MAX_LEN],Remark[MAX_LEN],UserID[MAX_LEN],
	Identit[MAX_LEN],Thing[MAX_LEN],StartTime[MAX_LEN],LastTime[MAX_LEN],Dcapacity[MAX_LEN],MultiMD[MAX_LEN],
	Time[MAX_LEN],UserPassword[MAX_LEN],Capacity[MAX_LEN],Result[MAX_LEN],StudentUserID[MAX_LEN],TeacherUserID[MAX_LEN],BeginTime[MAX_LEN],
	EndTime[MAX_LEN],Post[MAX_LEN],Course[MAX_LEN],StudentNumber[MAX_LEN],TestClassroom[MAX_LEN],
	Approver[MAX_LEN];   //C 变量 
	//use  ODBC  数据类型 
	//SQLINTEGER  cbAge=0,  cbSno=SQL_NTS,cbSex=SQL_NTS,cbName=SQL_NTS,cbDepart=SQL_NTS;//db cursor


	/* Step 2 初始化环境 */
	ret=SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&kinghenv);
	//ret=SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&serverhenv);
	ret=SQLSetEnvAttr(kinghenv,SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	//ret=SQLSetEnvAttr(serverhenv,SQL_ATTR_ODBC_VERSION,(void*)SQL_OV_ODBC3, 0);

	/* Step 3 建立连接 */
	ret=SQLAllocHandle(SQL_HANDLE_DBC, kinghenv, &kinghdbc);
	//ret=SQLAllocHandle(SQL_HANDLE_DBC, serverhenv,&serverhdbc);
	
	//提前配置 odbc:  DSN数据源名  数据库登录名 密码 
	ret=SQLConnect(kinghdbc,(unsigned char *)dsnname1, SQL_NTS,(unsigned char *)logname1,SQL_NTS,(unsigned char *)pwd1,SQL_NTS);
	//ret=SQLConnect(kinghdbc,(SQLCHAR *)"KingbaseES ODBC", SQL_NTS,(SQLCHAR *)"SYSTEM",SQL_NTS, (SQLCHAR *)"MANAGER",SQL_NTS);
	if (!SQL_SUCCEEDED(ret) )	/*连接失败时返回错误值*/
	{
		printf("connect1 error...\n");
		return -1;
	}
	else printf("connect1 success...\n");
	
	//ret=SQLConnect(serverhdbc,(unsigned char *)dsnname2, SQL_NTS,(unsigned char *)logname2,SQL_NTS,(unsigned char *)pwd2,SQL_NTS);
	//ret=SQLConnect(serverhdbc,(unsigned char *)"s2", SQL_NTS,(unsigned char *)"testlogin",SQL_NTS,(unsigned char *)"123",SQL_NTS);
	//ret=SQLConnect(serverhdbc, (SQLCHAR *)"SQLServer", SQL_NTS, (SQLCHAR *)"sa",SQL_NTS,(SQLCHAR *)"sa",SQL_NTS);

	/* Step 4 初始化语句句柄 */
	ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);
	ret=SQLSetStmtAttr(kinghstmt,SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)SQL_BIND_BY_COLUMN,SQL_IS_INTEGER);
    do
	{ 
	int n; 
	printf("\n****欢迎进入石大教室预约系统(管理员）****");
	printf("\n登录请输入1");
	printf("对不起打扰了，我选择退出系统输入0\n"); 
    scanf("%d",&n);
	switch(n){
		case 1:
			{
			getchar();
			printf("请输入你的管理员用户账号：");
			//char userid[50],userpassword;
			char str[1000] = "SELECT * FROM Manager_T where ManagerUserID=\'";
			gets(userid);
			printf("ID = %s\n", userid);
			printf("请输入你的用户密码:");
			gets(userpassword);
			printf("USERPASSWORD=%s\n",userpassword);
			int i=0,j=0;
	        SQLCHAR *sql=NULL;
	        while (str[i] !='\0')
            i++;
            while (userid[j] !='\0')
            str[i++]=userid[j++]; /* 拼接字符 */
            strcat(str,"\'");
            strcat(str,"and UserPassword=\'");
            strcat(str,userpassword);
            strcat(str,"\'");
            printf("%s",str);
            sql = (SQLCHAR *)str;
            ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);
            ret=SQLExecDirect(kinghstmt,sql,SQL_NTS);
            if (ret==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO) 
			{
				printf("SQLExecDirect  select  success...\n");
				ret=SQLBindCol(kinghstmt,1,SQL_C_CHAR,StudentUserID,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,2,SQL_C_CHAR,UserPassword,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,3,SQL_C_CHAR,UserEmail,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,4,SQL_C_CHAR,UserSex,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,5,SQL_C_CHAR,UserQuestion,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,6,SQL_C_CHAR,UserAnswer,MAX_LEN,NULL);
				printf("SQLBindCol success...\n");	
			}
			else  
			{
			printf("SQLExecDirect  select  error...\n");
	       return 0;//出现故障即刻停止 
	        }
			/* Step 6.2 处理结果集-游标,  逐行SQLFetch  */
			printf("Begin SQLFetch...\n");
			while ((ret=SQLFetch(kinghstmt))!=SQL_NO_DATA_FOUND) 
			{  
				if(ret==SQL_ERROR)	
				{
				printf("没有此用户请重新登录或者注册一个用户");break;
				
			    }
				else  	
				{
					printf("您已经登陆成功，欢迎使用教室预约系统！\n");judge=1;
				}
			}
			break;			
		}	
		case 0:
		return 0;
		default:
		printf("请输入正确的选项\n");
	}
}while(judge==0);
	do
	{
		printmenu();  //主菜单 
		scanf("%d",&t);
		switch(t)
		{	
			case 1:
				{
			ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);
			
			/*Step 5 直接执行SQL语句	*/
			ret=SQLExecDirect(kinghstmt,(SQLCHAR *)"SELECT * FROM  Student_T",SQL_NTS);
			getchar();
			/* Step 6.1 处理结果集-游标，将数据缓冲（C变量）绑定到结果集的各列 SQLBindCol  */ 
			if (ret==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO) 
			{
				printf("SQLExecDirect  select  success...\n");
				ret=SQLBindCol(kinghstmt,1,SQL_C_CHAR,StudentUserID,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,2,SQL_C_CHAR,UserPassword,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,3,SQL_C_CHAR,UserEmail,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,4,SQL_C_CHAR,UserSex,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,5,SQL_C_CHAR,UserQuestion,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,6,SQL_C_CHAR,UserAnswer,MAX_LEN,NULL);
				printf("SQLBindCol success...\n");	
			}
			else  
			{
			printf("SQLExecDirect  select  error...\n");
	        break;//出现故障即刻停止 
	        }
			/* Step 6.2 处理结果集-游标,  逐行SQLFetch  */
			printf("Begin SQLFetch...\n");
			while ((ret=SQLFetch(kinghstmt))!=SQL_NO_DATA_FOUND) 
			{  
				if(ret==SQL_ERROR)	printf("Fetch error\n");
				else  	
				{
					printf("学生学号：%s  学生密码：%s  邮箱：%s  性别:%s   密保问题：%s    密保答案：%s  \n",StudentUserID,UserPassword,UserEmail,UserSex,UserQuestion,UserAnswer);
				}
			}			
			break;
	}
	        case 2:
		        {
			ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);
			
			/*Step 5 直接执行SQL语句	*/
			ret=SQLExecDirect(kinghstmt,(SQLCHAR *)"SELECT * FROM  Teacher_T",SQL_NTS);
			getchar();
			/* Step 6.1 处理结果集-游标，将数据缓冲（C变量）绑定到结果集的各列 SQLBindCol  */ 
			if (ret==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO) 
			{
				printf("SQLExecDirect  select  success...\n");
				ret=SQLBindCol(kinghstmt,1,SQL_C_CHAR,TeacherUserID,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,2,SQL_C_CHAR,UserPassword,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,3,SQL_C_CHAR,UserEmail,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,4,SQL_C_CHAR,UserSex,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,5,SQL_C_CHAR,UserQuestion,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,6,SQL_C_CHAR,UserAnswer,MAX_LEN,NULL);
				printf("SQLBindCol success...\n");	
			}
			else  
			{
			printf("SQLExecDirect  select  error...\n");
	        break;//出现故障即刻停止 
	        }
			/* Step 6.2 处理结果集-游标,  逐行SQLFetch  */
			printf("Begin SQLFetch...\n");
			while ((ret=SQLFetch(kinghstmt))!=SQL_NO_DATA_FOUND) 
			{  
				if(ret==SQL_ERROR)	printf("Fetch error\n");
				else  	
				{
					printf("老师工号：%s  学生密码：%s  邮箱：%s  性别:%s   密保问题：%s    密保答案：%s  \n",StudentUserID,UserPassword,UserEmail,UserSex,UserQuestion,UserAnswer);
				}
			}			
			break;
		}
		    case 3:
			{
				ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);
			
			/*Step 5 直接执行SQL语句	*/
			ret=SQLExecDirect(kinghstmt,(SQLCHAR *)"SELECT * FROM  ApplyC_T",SQL_NTS);
			getchar();
			/* Step 6.1 处理结果集-游标，将数据缓冲（C变量）绑定到结果集的各列 SQLBindCol  */ 
			if (ret==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO) 
			{
				printf("SQLExecDirect  select  success...\n");
				ret=SQLBindCol(kinghstmt,1,SQL_C_CHAR,UserID,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,2,SQL_C_CHAR,Identit,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,3,SQL_C_CHAR,Thing,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,4,SQL_C_CHAR,StartTime,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,5,SQL_C_CHAR,LastTime,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,6,SQL_C_CHAR,Dcapacity,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,7,SQL_C_CHAR,MultiMD,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,8,SQL_C_CHAR,Time,MAX_LEN,NULL);
				ret=SQLBindCol(kinghstmt,9,SQL_C_CHAR,Result,MAX_LEN,NULL);
				printf("SQLBindCol success...\n");	
			}
			else  
			{
			printf("SQLExecDirect  select  error...\n");
	        break;//出现故障即刻停止 
	        }
			/* Step 6.2 处理结果集-游标,  逐行SQLFetch  */
			printf("Begin SQLFetch...\n");
			while ((ret=SQLFetch(kinghstmt))!=SQL_NO_DATA_FOUND) 
			{  
				if(ret==SQL_ERROR)	printf("Fetch error\n");
				else  	
				{
					printf("用户号：%s  身份：%s  事情：%s  开始时间：%s   结束时间：%s   需要容量：%s  是否需要多媒体：%s  申请时间：%s 结果：%s  \n",UserID,Identit,Thing,StartTime,LastTime,Dcapacity,MultiMD,Time,Result);
				}
			}			
			break;
		    }
			case 4:
				{
					printf("\n要更改用户的信息输入1,要删除用户信息输入2\n");
					int n;
					scanf("%d",&n);
					switch(n){
						case 1:
							{
							J=0;
							printf("请输入学生的用户名：\n");
							char StudentId[MAX_LEN];
							scanf("%s",StudentId);
                            inputStu();
                            int i=0,j=0;
                            SQLCHAR *sql = NULL;
                            char Userpass[100];
                            printf("请输入你的新密码！");
                            scanf("%s",Userpass);
                            char str[1000]="update Student_T  set UserPassword=\'";
							strcat(str,Userpass);
							strcat(str,"\'");
							strcat(str,",UserEmail=\'");
							strcat(str,UserEmail);
							strcat(str,"\'");
							strcat(str,",UserSex=\'");
							strcat(str,UserSex);
							strcat(str,"\'");
							strcat(str,",UserQuestion=\'");
							strcat(str,UserQuestion);
							strcat(str,"\'");
							strcat(str,",UserAnswer=\'");
							strcat(str,UserAnswer);
							strcat(str,"\' where StudentUserID=\'");
							strcat(str,StudentId);
							strcat(str,"\'");
							printf("\nupdate:%s\n",str);
							sql = (SQLCHAR *)str;
							s.cbStudentUserID = SQL_NTS;
							s.cbUserPassword = SQL_NTS;
							s.cbUserEmail = SQL_NTS;
							s.cbUserSex = SQL_NTS;
							s.cbUserQuestion = SQL_NTS;
							s.cbUserAnswer = SQL_NTS;
							ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);  
							ret = SQLExecDirect(kinghstmt, sql, SQL_NTS);
							break; 
							}
							case 2:
								{		
							printf("请输入非法学生的用户名：\n");
							char StudentId[MAX_LEN];
							scanf("%s",StudentId);
                            SQLCHAR *sql = NULL;
                            char str[1000]="delete from Student_T where StudentUserID=\'";
							strcat(str,StudentId);
							strcat(str,"\'");
							printf("\nupdate:%s\n",str);
							sql = (SQLCHAR *)str;
							s.cbStudentUserID = SQL_NTS;
							s.cbUserPassword = SQL_NTS;
							s.cbUserEmail = SQL_NTS;
							s.cbUserSex = SQL_NTS;
							s.cbUserQuestion = SQL_NTS;
							s.cbUserAnswer = SQL_NTS;
							ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);  
							ret = SQLExecDirect(kinghstmt, sql, SQL_NTS);
							break; 
								}
					}
					break;
				}
				case 5:
				{
					printf("\n要更改用户的信息输入1,要删除用户信息输入2\n");
					int n;
					scanf("%d",&n);
					switch(n){
						case 1:
							{
							J=1;
							printf("请输入老师的用户名：\n");
							char TeacherId[MAX_LEN];
							scanf("%s",TeacherId);
                            inputStu();
                            int i=0,j=0;
                            SQLCHAR *sql = NULL;
                            char Userpass[100];
                            printf("请输入你的新密码！");
                            scanf("%s",Userpass);
                            char str[1000]="update Teacher_T  set UserPassword=\'";
							strcat(str,Userpass);
							strcat(str,"\'");
							strcat(str,",UserEmail=\'");
							strcat(str,UserEmail);
							strcat(str,"\'");
							strcat(str,",UserSex=\'");
							strcat(str,UserSex);
							strcat(str,"\'");
							strcat(str,",UserQuestion=\'");
							strcat(str,UserQuestion);
							strcat(str,"\'");
							strcat(str,",UserAnswer=\'");
							strcat(str,UserAnswer);
							strcat(str,"\'");
							strcat(str,",Post=\'");
							strcat(str,UserPost);
							strcat(str,"\' where TeacherUserID=\'");
							strcat(str,TeacherId);
							strcat(str,"\'");
							printf("\nupdate:%s\n",str);
							sql = (SQLCHAR *)str;
							s.cbTeacherUserID = SQL_NTS;
							s.cbUserPassword = SQL_NTS;
							s.cbUserEmail = SQL_NTS;
							s.cbUserSex = SQL_NTS;
							s.cbUserQuestion = SQL_NTS;
							s.cbUserAnswer = SQL_NTS;
							s.cbPost = SQL_NTS;
							ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);  
							ret = SQLExecDirect(kinghstmt, sql, SQL_NTS);
							break; 
							}
							case 2:
								{		
							printf("请输入非法老师的用户名：\n");
							char TeacherID[MAX_LEN];
							scanf("%s",TeacherID);
                            SQLCHAR *sql = NULL;
                            char str[1000]="delete from Teacher_T where TeacherUserID=\'";
							strcat(str,TeacherID);
							strcat(str,"\'");
							printf("\nupdate:%s\n",str);
							sql = (SQLCHAR *)str;
							s.cbStudentUserID = SQL_NTS;
							s.cbUserPassword = SQL_NTS;
							s.cbUserEmail = SQL_NTS;
							s.cbUserSex = SQL_NTS;
							s.cbUserQuestion = SQL_NTS;
							s.cbUserAnswer = SQL_NTS;
							s.cbPost = SQL_NTS;
							ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);  
							ret = SQLExecDirect(kinghstmt, sql, SQL_NTS);
							break; 
								}
					}
					break;
				}
			case 6:
				{
		ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);
	    ret=SQLSetStmtAttr(kinghstmt,SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)SQL_BIND_BY_COLUMN,SQL_IS_INTEGER);
			/*Step 5 预处理再执行SQL语句	*/
			/*Step 5  1.预编译带有参数的语句  SQLPrepare、SQLBindParameter*/
			ret=SQLPrepare(kinghstmt,(SQLCHAR *)"insert into ReserveC_T(UserID,ClassID,Approver,Time,Result) values(?, ?, ?, ?, ?)", SQL_NTS);
			if (ret==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO)
			{
				//SQLBindParameter:用于绑定参数的ODBC API函数:执行参数化查询，需要将语句中各个参数对应的变量缓存，与Statement句柄实现绑定。
     			ret=SQLBindParameter(kinghstmt,1,SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR,MAX_LEN,0,UserID,0,NULL); 
				ret=SQLBindParameter(kinghstmt,2,SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR,MAX_LEN,0,ClassID,0,NULL);
				ret=SQLBindParameter(kinghstmt,3,SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR,MAX_LEN,0,Approver,0,NULL);
				ret=SQLBindParameter(kinghstmt,4,SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR,MAX_LEN,0,Time,0,NULL);
				//ret=SQLBindParameter(kinghstmt,4,SQL_PARAM_INPUT, SQL_C_LONG,SQL_INTEGER,0,0,&sAge,0,NULL);		
				ret=SQLBindParameter(kinghstmt,5,SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR,MAX_LEN,0,Result,0,NULL);
				printf("SQLBindParameter success...\n");
			}
			else 
			{ printf("SQLBindParameter error...\n");
			break;//错误退出按钮 
	     	}
			//用户输入 参数 
			/*Step 5  2. 执行预编译后的SQL语句  SQLExecute  多次*/	
			while (1) 
			{  
				printf("input UserID ClassID Approver Time Result:\n");
				scanf("%s%s%s%s%s",UserID,ClassID,Approver,Time,Result);
				printf("you input :%s %s %s %s %s\n",UserID,ClassID,Approver,Time,Result);
				ret=SQLExecute(kinghstmt);
				if(ret==SQL_ERROR)
				{
					printf("insert error...\n");
					break;
				}				
				else if(ret==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO)
						printf("insert success...\n");
				else printf("insert ???...\n");break;
				getchar();
				printf("go on ?");
				scanf("%d",&k);//1 0
				if(k==0) break;		
			}
			break;
		}
		case 7:
			{
				ret=SQLAllocHandle(SQL_HANDLE_STMT,kinghdbc,&kinghstmt);
	    ret=SQLSetStmtAttr(kinghstmt,SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)SQL_BIND_BY_COLUMN,SQL_IS_INTEGER);
			/*Step 5 预处理再执行SQL语句	*/
			/*Step 5  1.预编译带有参数的语句  SQLPrepare、SQLBindParameter*/
			ret=SQLPrepare(kinghstmt,(SQLCHAR *)"insert into Exam_T(BeginTime,EndTime,Course,StudentNumber,TestClassroom) values(?, ?, ?, ?, ?)", SQL_NTS);
			if (ret==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO)
			{
				//SQLBindParameter:用于绑定参数的ODBC API函数:执行参数化查询，需要将语句中各个参数对应的变量缓存，与Statement句柄实现绑定。
     			ret=SQLBindParameter(kinghstmt,1,SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR,MAX_LEN,0,BeginTime,0,NULL); 
				ret=SQLBindParameter(kinghstmt,2,SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR,MAX_LEN,0,EndTime,0,NULL);
				ret=SQLBindParameter(kinghstmt,3,SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR,MAX_LEN,0,Course,0,NULL);
				ret=SQLBindParameter(kinghstmt,4,SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR,MAX_LEN,0,StudentNumber,0,NULL);
				//ret=SQLBindParameter(kinghstmt,4,SQL_PARAM_INPUT, SQL_C_LONG,SQL_INTEGER,0,0,&sAge,0,NULL);		
				ret=SQLBindParameter(kinghstmt,5,SQL_PARAM_INPUT, SQL_C_CHAR,SQL_CHAR,MAX_LEN,0,TestClassroom,0,NULL);
				printf("SQLBindParameter success...\n");
			}
			else 
			{ printf("SQLBindParameter error...\n");
			break;//错误退出按钮 
	     	}
			//用户输入 参数 
			/*Step 5  2. 执行预编译后的SQL语句  SQLExecute  多次*/	
			while (1) 
			{  
				printf("input BeginTime EndTime Course StudentNumber TestClassroom:\n");
				scanf("%s%s%s%s%s",BeginTime,EndTime,Course,StudentNumber,TestClassroom);
				printf("you input :%s %s %s %s %s\n",BeginTime,EndTime,Course,StudentNumber,TestClassroom);
				ret=SQLExecute(kinghstmt);
				if(ret==SQL_ERROR)
				{
					printf("insert error...\n");
					break;
				}				
				else if(ret==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO)
						printf("insert success...\n");
				else printf("insert ???...\n");break;
				getchar();
				printf("go on ?");
				scanf("%d",&k);//1 0
				if(k==0) break;		
			}
			break;
			}
	

	case 0:  
				/* Step 7 中止处理*/
				SQLFreeHandle(SQL_HANDLE_STMT,kinghstmt);
				SQLDisconnect(kinghdbc);
				SQLFreeHandle(SQL_HANDLE_DBC,kinghdbc);
				SQLFreeHandle(SQL_HANDLE_ENV,kinghenv);
				printf("SQLFree.\n ");
				return 0;
	default:
	{
	printf("请输入正确的选项\n");
	break;
	}		
		}
	} while(1); 
	return 0;
}
