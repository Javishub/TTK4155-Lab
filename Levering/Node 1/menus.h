#define MAX_SUBMENUS

int highscore_score = 0;
int prev_highscore = 0;

typedef struct Menu {
	char * text;
	void (*function)();
	struct Menu * parent;
	struct Menu * subMenu[MAX_SUBMENUS];
	}menu_t, *menu_ptr;

menu_ptr menu_add(menu_ptr parent, char * text, void(*function)()){
	menu_ptr subMenu = malloc(sizeof(menu_t));
	subMenu->text = text;
	subMenu->function = function;
	subMenu->parent = parent;
	
	int i=0;
	while (parent->subMenu[i] !=NULL){
		i++;
	}
	parent->subMenu[i] = subMenu;
	return subMenu;
}

void update_highscore(int scr){
	if(scr >= prev_highscore){
		highscore_score = scr;
		prev_highscore = highscore_score;
	}
}


void show_highscore(int arr[]){
	oled_pos(4,32);
	oled_print("Highscore: ");
	oled_print_int(highscore_score);
}

void playgame()
{
}

void game_loss(){
	oled_reset();
	oled_print("You suck");
	oled_pos(1,0);
	oled_print("Therefore you lose!");
}

void won_game(){
	oled_pos(0,0);
	oled_clear_line(0);
	oled_print("YOU WON!");
}

void playing_game(int scored){
	oled_reset();
	oled_pos(0,0);
	oled_print("Score :");
	oled_print_int(scored);
}