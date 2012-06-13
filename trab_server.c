
#include "trab.h"
int current_user;
struct user_ip{
	char name[60];	
	char ip[30];
};



//fazer parse de um arquivo que contenha o endereco ip dos hosts
struct user_ip list[10];

int has_ask = 0;

form ask;

control *
whatdoto_1_svc(control *argp, struct svc_req *rqstp)
{
	static control  result;
	if(has_ask)
	{
		result.attr.booleanVar = 1;
	}
	else
	{
		result.attr.booleanVar = 0;
	}

	return &result;
}

control *
checkhost_1_svc(control *argp, struct svc_req *rqstp)
{
	static control  result;


	return &result;
}

form *
sendask_1_svc(form *argp, struct svc_req *rqstp)
{
	static form  result;
	if(argp->attr.booleanVar)
	{
		memcpy(&ask,argp,sizeof(form));
		has_ask = 1;
	}
	else
	{
		memcpy(&result,&ask,sizeof(form));
	}

	return &result;
}

form *
sendanswer_1_svc(form *argp, struct svc_req *rqstp)
{
	static form  result;

	//manda pra deus e pro mundo

	return &result;
}

infoperson *
nicetomeetyou_1_svc(infoperson *argp, struct svc_req *rqstp)
{
	static infoperson  result;

	printf("%d\n",current_user);

	return &result;
}
