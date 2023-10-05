#define MAX_SUBMENUS

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

void show_highscore(){
	oled_print("Highscoresssss");
}

void playgame()
{
	oled_print("Play game 1 or game 2?");
}

void play_game1(){
	oled_print("game1");
}

void play_game2(){
	oled_print("game2");
}
