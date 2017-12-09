void update_actor(ALL_A *actor, char *option, int serial){
	FILE *act_l;
	act_l = fopen("actor_log","a");
	LINK_D *tmp = director->head;
	LINK_D *update;
	while(tmp->next_actor != actor->tail){
		tmp = tmp->next_actor;
		if(tmp->serial_num == serial){
			update = tmp;
			break;
		}
	}
	int check = strlen(option);
	char *check_option = (char*)calloc(4, sizeof(char));
	char *revise = (char*)malloc(sizeof(char)*2);
	char *str;
	char c;
	str = (char*)calloc(30, sizeof(char));
	int num;
	for(int i=0; i<check; i++){
		if(*(option+i) == 'n') *(check_option+0) = 'n';
		if(*(option+i) == 's') *(check_option+1) = 's';
		if(*(option+i) == 'b') *(check_option+2) = 'b';
		if(*(option+i) == 'm') *(check_option+3) = 'm';
	}
	fprintf(act_l, "update:%d:", serial);
	if(*(check_option+0) == 'n'){
	       printf("name > ");
	       scanf("%[^\n]", str);
	       getchar();
	       if((strcmp(str, update->name))==0){
		       printf("You have the same record.\n");
		       printf("name : %s\n", update->name);
		       printf("Do you want to revise?(Y : yes, N: no)\n");
		       scanf("%s", revise);
		       getchar();
		       if((strcmp(revise, "Y") == 0)) fprintf(act_l, "%s:", update->name);
		       else if((strcmp(revise, "N")==0)) fprintf(act_l, "=:");
	       }
	       else fprintf(act_l, "%s:", str);
	}
	else if(*(check_option+0) == '\0') fprintf(act_l, "=:");
	if(*(check_option+1) == 's'){
		printf("sex > ");
                scanf("%c", c);
                getchar();
                if(c == update->sex){
			printf("You have the same record.\n");
                        printf("sex : %c\n", update->sex);
                        printf("Do you want to revise?(Y : yes, N: no)\n");
                        scanf("%s", revise);
			getchar();
                        if(strcmp(revise, "Y")==0) fprintf(act_l, "%c:", update->sex);
			else if((strcmp(revise, "N")==0)) fprintf(act_l, "=:");
		}
                else fprintf(act_l, "%c:", c);
	}
	else if(*(check_option+1) == '\0') fprintf(act_l, "=:");
	if(*(check_option+2) == 'b'){
		printf("birth > ");
                scanf("%d", &num);
		char *snum = (char*)calloc(2, sizeof(char));
		change_num_to_string(snum, num);
                if(update->birth == num){
			printf("You have the same record.\n");
                        printf("birth : %d\n", update->year);
                        printf("Do you want to revise?(Y : yes, N: no)\n");
                        scanf("%s", revise);
			getchar();
                        if(strcmp(revise, "Y")==0) fprintf(act_l, "%d:", update->birth);
			else if((strcmp(revise, "N")==0)) fprintf(act_l, "=:");
		}
                else fprintf(act_l, "%d:", num);
	}
	else if(*(check_option+2) == '\0') fprintf(act_l, "=:");
        if(*(check_option+5) == 'm'){
	       printf("movies > ");
	       
	       LINK_A *ttmp = (LINK_A*)malloc(sizeof(LINK_A));
	       input_multiple_act(ttmp);

	       int a=1, b=1;
	       LINK_W *new;
	       new = ttmp->title;

	       while(new ->next != NULL){
		       a++;
		      new = new->next;
	       }
	       new = ttmp->title;
	       LINK_W *org;
	       org = update->title;
	       while(org->next != NULL){
		       b++;
		      org = org->next;
	       }
	       org = update->title;
	       if(a != b){
		       while(new->next != NULL){
			       fprintf(act_l, "%s,", new->data);
			       new = new->next;
		       }
		       fprintf(act_l, "%s\n", new->data);
	       }
	       else{
		       char *gar1 = (char*)calloc(200, sizeof(char));
		       char *gar2 = (char*)calloc(200, sizeof(char));
		       while(new->next != NULL){
			       strcpy(gar1+strlen(gar1), new->data);
			       strcat(gar1, ",");
			       new = new->next;
		       }
		       strcat(gar1, new->data);
		       while(org->next != NULL){
			       strcpy(gar2+strlen(gar2), org->data);
			       strcat(gar2, ",");
			       org = org->next;
		       }
		       strcat(gar2, org->data);
		       new = ttmp->title;
		       org = update->title;
		       if(strcmp(gar1, gar2)==0){
			       printf("You have the same records.\n");
			       printf("movies : %s\n", gar1);
			       printf("Do you want to revise?(Y : yes, N : no)\n");
			       scanf("%s", revise);
			       getchar();
			       if(strcmp(revise, "Y")==0){
				       while(org->next != NULL){
					       fprintf(act_l, "%s,", org->data);
					       org = org->next;
				       }
				       fprintf(act_l, "%s\n", org->data);
			       }
			       else if(strcmp(revise, "N")==0) fprintf(act_l, "=\n");
		       }
		       else{
			       while(new->next != NULL){
				       fprintf(act_l, "%s,", new->data);
				       new = new->next;
			       }
			       fprintf(act_l, "%s\n", new->data);
		       }
	       }
		       
	}
	else if(*(check_option+5) == '\0') fprintf(act_l, "=\n");
	fclose(dir_l);
}

void update_actor_list(ALL_A *actor,FILE *act_l){
	char *read_data = (char*)calloc(30, sizeof(char));
	int read_num;
	char read_one;
	fscanf(act_l, "%*c%d", &read_num);

	LINK_A *tmp = (LINK_A*)malloc(sizeof(LINK_A));
	tmp = actor->head;
	LINK_A *update = (LINK_A*)malloc(sizeof(LINK_A));
	while(tmp->next_actor != actor->tail){
		tmp = tmp->next_actor;
		if(tmp->serial_num == read_num){
			update = tmp;
			break;
		}
	}
	update->serial_num = read_num;

	fscanf(act_l, "%*c%[^:]", read_data);
	if(strcmp(read_data, "=")!=0)
		strcpy(update->name, read_data);

	fscanf(act_l, "%*c%c", &read_one);
	if(read_one != '=')
		update->sex = read_one;

	fscanf(act_l, "%*c%[^:]", read_data);
	if(strcmp(read_data, "=")!=0){
		read_num = atoi(read_data);
		update->birth = read_num;
	}

	char *temp = (char*)calloc(300, sizeof(char));
	fscanf(act_l, "%[^\n]%*c", temp);
	if(strcmp(temp, "=")!=0){
		LINK_W *h = (LINK_W*)malloc(sizeof(LINK_W));
		LINK_W *l = (LINK_W*)malloc(sizeof(LINK_W));
		h = update->title;
		while(h->next != NULL){
			LINK_W *d = (LINK_W*)malloc(sizeof(LINK_W));
			d = h;
			l = d->next;
			free(d->data);
			free(d);
			h = l;
		}
		free(l->data);
		free(l);
		update->title = (LINK_W *)malloc(sizeof(LINK_W));
		h->data = strtok(temp, ",");
		while(h->data != NULL){
			char *check = (char*)malloc(sizeof(char)*30);
			check = strtok(NULL, ",");
			if(check == NULL){
				h->next = NULL;
				break;
			}
			else{
				h->next = (LINK_W *)malloc(sizeof(LINK_W));
				h = h->next;
				h->next = NULL;
				h->data = check;
			}
		}
	}
}

