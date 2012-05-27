struct commomattributtes{
	char address[40];
	int booleanVar;
};

struct control{
	int action;
	commomattributtes attr;
};

struct form{
	int ask_id;
	char ask[140];
	char options[140];
	int answer;
	int next;
	commomattributtes attr;
};

struct infoperson{
	char name[40];
	int age;
	commomattributtes attr;
};

program PROGJOGO{
	version VERJOGO{
		control whatdoto(control) = 1;
		control checkhost(control) = 2;
		form sendask(form) = 3;
		form sendanswer(form) = 4;
		infoperson nicetomeetyou(infoperson)=5;
	} =1;
} = 0x10232143;



