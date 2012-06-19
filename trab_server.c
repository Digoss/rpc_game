#include "trab.h"
#include "trab_clnt.c"
#include <string.h>

int gamestarted = 1;

int has_ask = 0; //verifica se o servidor ja recebeu as 3 perguntas do coordenador

int questions_answered = 0; //verifica quantas perguntas o player ja respondeu

form ask[3]; //perguntas enviadas pelo coordenador

form answer[10]; //respostas dadas ao coordenador

int count_answer[10]; //marcador dos players que respoderao

int champions[10]; //vetor com a ordem dos vencedores

int responses[3]; //respostas das perguntas, guardada pelo coordenador

int reset; //quando player detecta que coordenador nao esta mais ativo

int was_send; //quando coordenador enviou pelo menos uma mensagem

int current_user; //usuario atual do host

int manager = -1; //coordenador do jogo

int has_new_player; //verifica se existem novos players na partida

int finished; //verifica quando player ja respondeu e recebeu a confimacao com sucesso

struct user_ip{
	char name[60];
	int age;
	char ip[40];
	int points;
	int keepAlive;
	int marked;
};

struct user_ip list[10]; //lista de ips

int
clear_all()
{
	//gamestarted = 1;

	has_ask = 0; //verifica se o servidor ja recebeu as 3 perguntas do coordenador

	questions_answered = 0; //verifica quantas perguntas o player ja respondeu

	//form ask[3]; //perguntas enviadas pelo coordenador

	//form answer[10]; //respostas dadas ao coordenador

	int i;
	for(i=0;i<10;i++)
		count_answer[i] = 0; //marcador dos players que respoderao

	//int responses[3]; //respostas das perguntas, guardada pelo coordenador

	//reset = 0; //quando player detecta que coordenador nao esta mais ativo

	was_send = 0; //quando coordenador enviou pelo menos uma mensagem

	//int current_user; //usuario atual do host

	//int manager = -1; //coordenador do jogo

	has_new_player = 0; //verifica se existem novos players na partida

	finished = 0;
}

int
readPlayers()
{
	int index;
	FILE *fr;
        char * line = malloc(140);
	int size_ip;
	int i = 0;

        fr = fopen ("players.txt","rt");
	index = 0;
        while(fgets(line,60,fr) != NULL)
        {
		strcpy(list[index].name, line);
		if(fgets(line,5,fr) != NULL)
		{
			list[index].age =  atoi(line);
			if(fgets(line,40,fr) != NULL)
			{
				strcpy(list[index].ip, line);
				size_ip = strlen(list[index].ip);
				list[index].ip[size_ip] = '\0';
			}
		}
                index++;
        }
	return 0;
}


int
find_by_address(commomattributtes attr)
{
	int i;
	for(i=0;i<10;i++)
		if(strcmp(attr.address,list[i].ip)==0)
			return i;
	printf("Endereco nao encontrado\n");
	return -1;
}

void
bubble_sort_game()
{
	//verificar se existe players nao marcados
	//champions
	int max;
	int i,j;
	for(i=0;i<10;i++)
	{
		max = 0;
		for(j=0;j<10;j++)
		{
			if(list[max].points < list[j].points)
			{
				if(list[j].marked == 0)
				{
					max = j;
				}
			}
			else if(list[max].points == list[j].points)
			{
				if(i > j)
				{	
					if(list[j].marked == 0)
					{
						max = j;
					}
				}
			}
		}
		list[max].marked = 1;
		champions[i] = max;
	}
}

int
check_end()
{
	int all_returned = 1;
	int i;
	for(i=0;i<10;i++) //verifica se todos os que receberam a pergunta ja a responderam
		if(list[i].keepAlive)
			if(count_answer[i] == 0)
				all_returned = 0;
	if(all_returned)
	{
		for(i=0;i<10;i++)
		{
			plus_point(i);
		}
		bubble_sort_game();
		printf("Vencedores\n\n");
		//dar print do coordenador
		for(i=0;i<5;i++)
		{
			printf("Nome: %s",list[champions[i]].name);
			printf("Idade: %d",list[champions[i]].age);
			printf("Pontos: %d\n",list[champions[i]].points);
		}
		printf("Nao classificados\n\n");
		for(;i<10;i++)
		{
			printf("Nome: %s",list[champions[i]].name);
			printf("Idade: %d",list[champions[i]].age);
			printf("Pontos: %d",list[champions[i]].points);
		}
		//fim de jogo
	}
	return 1;
}

int
plus_point(int index)
{
	int i;
	for(i=0;i<3;i++)
	{
		if(responses[i] == answer[index].answer[i])
		{
			list[index].points = 1 + list[index].points;
		}
	}
}

//fazer parse de um arquivo que contenha o endereco ip dos hosts


control *
whatdoto_1_svc(control *argp, struct svc_req *rqstp)
{
	static control  result;
	if(reset) //se host atual detectou que coordenador caiu
	{
		clear_all();
		result.action = 100;
		reset = 0;
	}
	else if((current_user == manager && was_send == 0) || has_new_player) //envia perguntas caso seja coordenador e nao envio as perguntas
	{									// ou se existe um novo usuario
		result.action = 1;
		has_new_player = 0;
	}
	else if(has_ask == 3 && questions_answered < 3) // requisita resposta quando tem as perguntas e nao forao todas respondidas
	{
		result.action = 2;
	}
	else if(questions_answered == 3 && finished == 0) // requita fim de jogo
	{
		finished = 1;
		result.action = 3;
	}
	else if(current_user == manager)
	{
		result.action = 4;
	}
	else
	{
		result.action = 0;
	}

	return &result;
}

control *
checkhost_1_svc(control *argp, struct svc_req *rqstp)
{	
	printf("Game %d", gamestarted);
	if(gamestarted) //se game ainda nao comecou para o usuario corrente, le arquivo de jogadores e inicia partida
	{
		printf("Jogo comecando\n");
		readPlayers();
		gamestarted = 0;
		printf("Nome: %sIdade: %d\nIP: %s\n",list[current_user].name,list[current_user].age,list[current_user].ip);
	}
	static control  result;
	CLIENT *clnt;

	control  *result_2;
        control  checkhost_1_arg;
	
	if(argp->action == 100) //outro servidor avisa que processo corrente e o de maior prioridade vivo achado por ele
	{
		printf("Fui avisado que sou o novo coordenador\n");
		int i;
		char *host;
		strcpy(checkhost_1_arg.attr.address, list[current_user].ip);
		for(i=0;i<0;i++)
		{	
			if(i != current_user)
			{
				host = list[i].ip;
				clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
                               	if (clnt == NULL) {
                               		 continue;
                       		}
				checkhost_1_arg.attr.booleanVar = 1;
				result_2 = checkhost_1(&checkhost_1_arg, clnt);
		        	if (result_2 == (control *) NULL) {
					continue;
        			}
				printf("Msg enviada para %s", host);
				//enviar mensagem avisando que agora o processo corrente e o coordenador
			}
		}
		has_new_player = 1;
		manager = current_user;
	}
	else if(argp->attr.booleanVar)//quando recebe aviso do novo coordenador 
	{	
		manager = find_by_address(argp->attr);
		printf("Recebi msg de %d, dizendo que ele e o novo coordenador\n",manager);
		clear_all();
	}
	else// quando recebe mensagem originada por um cliente
	{
		printf("Cliente detectou que coordenador esta fora\n");
		char *host;
		int i;
		for(i=0;i<10;i++)
		{
			if(i == current_user) //quando processo corrente e o de maior prioridade
			{
				manager = current_user;
				printf("Sou o player de mais alta prioridade\n");
				strcpy(checkhost_1_arg.attr.address, list[current_user].ip);
				for(i=0;i<0;i++)
				{	
					if(i != current_user)
					{
						host = list[i].ip;
						clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
	                                	if (clnt == NULL) {
							printf("Host %s esta fora\n",host);
       	                                		continue;
       	                        		}
						checkhost_1_arg.attr.booleanVar = 1;
						result_2 = checkhost_1(&checkhost_1_arg, clnt);
				        	if (result_2 == (control *) NULL) {
							printf("Host %s esta fora\n",host);
							continue;
		        			}
						//enviar mensagem avisando que agora o processo corrente e o coordenador
					}
					printf("Enviei msg para %s",host);
				}
				break;
			}
			else if(i != current_user)// procura o processo de maior prioridade e envia mensagem
			{
				host = list[i].ip;
				printf("Verificando que %s esta vivo para ser o novo coordenador\n",host);
				clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
				if (clnt == NULL) {
					printf("oi");
					continue;
		        	}
				checkhost_1_arg.action = 100; //avisa ao host destino que e o coordenador
				result_2 = checkhost_1(&checkhost_1_arg, clnt);
		        	if (result_2 == (control *) NULL) {
					printf("tchau");
					continue;
		        	}
				printf("%s sera o novo coordenador\n",host);
				break;
			}
		}
	}

	return &result;
}

form *
sendask_1_svc(form *argp, struct svc_req *rqstp)
{
	static form  result;

	if(current_user == manager) // se processo corrente e o coordenador
	{
		
		CLIENT *clnt;
		form  *result_3;
		form  sendask_1_arg;

		memcpy(&sendask_1_arg, argp, sizeof(form));
		responses[sendask_1_arg.next] = sendask_1_arg.answer[0];//armazena a resposta
		char *host;
		int i;
		for(i=0;i<10;i++)
		{
			if(current_user != i)
			{
				host = list[i].ip;
				clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
				if (clnt == NULL) {
					//clnt_pcreateerror (host);
					//exit (1);
					continue;
				}
				//send msg
				sendask_1_arg.attr.booleanVar = 1;
				result_3 = sendask_1(&sendask_1_arg, clnt);
				if (result_3 == (form *) NULL) {
					//clnt_perror (clnt, "call failed");
					continue;
				}
				was_send = 1;
				list[i].keepAlive = 1;
				//manter um array com os participantes que receberao a pergunta
			}
		}
	}
	if(argp->attr.booleanVar)
	{
		memcpy(&ask[argp->next],argp,sizeof(form));//caso eu esteja recebendo uma questao do grandao
		has_ask++; 
	}
	else
	{
		memcpy(&result,&ask[argp->next],sizeof(form));//caso eu esteja recebendo um requisicao do cliente
	}

	return &result;
}

form *
sendanswer_1_svc(form *argp, struct svc_req *rqstp)
{
	static form  result;

	
	if(current_user == manager) //se processo corrente e o coordenador recebendo respostas
	{
		if(find_by_address(argp->attr) >= 0)
		{
			int index = find_by_address(argp->attr);
			int formsize = sizeof(form);
			memcpy(answer+index*formsize, argp, formsize);
			count_answer[index] = 1;
			check_end();
		}
	}
	else //se processo corrente recebe resposta do cliente, a envia para o coordenador
	{
		memcpy(&answer[current_user],argp,sizeof(form));

		CLIENT *clnt;
		form  *result_4;
		form  sendanswer_1_arg;

		memcpy(&sendanswer_1_arg, &answer[current_user], sizeof(form));

		char *host = list[manager].ip;
		clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
		if (clnt == NULL) {
			reset = 1;
		}
		strcpy(sendanswer_1_arg.attr.address, list[current_user].ip);
		result_4 = sendanswer_1(&sendanswer_1_arg, clnt);
		if (result_4 == (form *) NULL) {
			reset = 1;
		}
		//manda pro grandao
		//falta tratamento caso o coordenador esteja morto
	}
	return &result;
}

infoperson *
nicetomeetyou_1_svc(infoperson *argp, struct svc_req *rqstp)
{
	static infoperson  result;
	if(argp->attr.booleanVar != 1)
	{
		CLIENT *clnt;
		infoperson  *result_5;
		 infoperson  nicetomeetyou_1_arg;
	
		char *host;
		int i;
		nicetomeetyou_1_arg.attr.booleanVar = 1;
		for(i=0;i<10;i++)
		{
			if(current_user != i)
			{
				host = list[i].ip;
	
				clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
				if (clnt == NULL) {
					list[i].keepAlive = 0;
					check_end();	
				}

				result_5 = nicetomeetyou_1(&nicetomeetyou_1_arg, clnt);
                        	if (result_5 == (infoperson *) NULL) {
					list[i].keepAlive = 0;
					check_end();
				}
			}
		}
	}
	return &result;
}

